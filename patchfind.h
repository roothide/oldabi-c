//
// Copyright (c) 2026 Nightwind
//

#ifndef PATCHFIND_H
#define PATCHFIND_H

#include <mach/mach.h>
#include <mach-o/dyld.h>

__attribute__((visibility("hidden")))
void *_Nullable patchfind_in_region(
	const vm_address_t start_addr,
	const vm_offset_t region_length,
	const unsigned char *_Nonnull const bytes_to_search,
	const unsigned char *_Nullable const byte_mask,
	const size_t byte_count
);

#endif // PATCHFIND_H
