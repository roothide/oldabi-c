//
// Copyright (c) 2026 Nightwind
// Original implementation: https://github.com/tealbathingsuit/OldABI/blob/main/patchfind.m
//

#include <mach-o/dyld.h>
#include <mach/mach.h>

__attribute__((always_inline))
static int memcmp_masked(
	const void *_Nonnull const ptr1,
	const void *_Nonnull const ptr2,
	const unsigned char *_Nonnull const mask,
	const size_t n)
{
    const unsigned char *const p = (const unsigned char *)ptr1;
    const unsigned char *const q = (const unsigned char *)ptr2;

    if (p == q) return 0;

    for (int i = 0; i < n; i++) {
        const unsigned char c_mask = mask[i];
        if ((p[i] & c_mask) != (q[i] & c_mask)) {
            // we do not care about 1 / -1
            return -1;
        }
    }

    return 0;
}

__attribute__((visibility("hidden")))
void *_Nullable patchfind_in_region(
	const vm_address_t start_addr,
	const vm_offset_t region_length,
	const unsigned char *_Nonnull const bytes_to_search,
	const unsigned char *_Nullable const byte_mask,
	const size_t byte_count
) {
    if (byte_count < 1) {
        return NULL;
    }

    unsigned int first_byte_index = 0;
    if (byte_mask != NULL) {
        for (unsigned int i = 0; i < byte_count; i++) {
            if (byte_mask[i] == 0xFF) {
                first_byte_index = i;
                break;
            }
        }
    }

    const unsigned char first_byte = bytes_to_search[first_byte_index];
    vm_address_t current_addr = start_addr;

    while (current_addr < start_addr + region_length) {
        const size_t search_size = (start_addr - current_addr) + region_length;
        void *found_ptr = memchr((void *)current_addr, first_byte, search_size);

        if (found_ptr == NULL) {
            break;
        }

        const vm_address_t found_addr = (vm_address_t)found_ptr;

        // correct found_ptr in respect of first_byte_index
        found_ptr = (void *)((intptr_t)found_ptr - first_byte_index);

        const size_t remaining_bytes = region_length - (found_addr - start_addr);

        if (remaining_bytes >= byte_count) {
            int memcmp_res;

            if (byte_mask != NULL) {
                memcmp_res = memcmp_masked(found_ptr, bytes_to_search, byte_mask, byte_count);
            } else {
                memcmp_res = memcmp(found_ptr, bytes_to_search, byte_count);
            }

            if (memcmp_res == 0) {
                return found_ptr;
            }
        } else {
            break;
        }

        current_addr = found_addr + 1;
    }

    return NULL;
}
