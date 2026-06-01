//
// Copyright (c) 2026 Nightwind
// Original implementation: https://github.com/tealbathingsuit/OldABI/blob/main/OldABI.swift
//

#include <mach-o/dyld.h>
#include <mach-o/fat.h>

static bool thin_header_looks_legacy(const struct mach_header *_Nonnull const header) {
    if (header->magic == MH_MAGIC_64 || header->magic == MH_CIGAM_64) {
        if ((header->cpusubtype & ~CPU_SUBTYPE_MASK) == CPU_SUBTYPE_ARM64E) {
            if (!(header->cpusubtype & CPU_SUBTYPE_PTRAUTH_ABI)) {
                return true;
            }
        }
    }

    return false;
}

__attribute__((visibility("hidden")))
bool header_looks_legacy(const struct mach_header *_Nonnull const header) {
    if (header->magic == FAT_CIGAM || header->magic == FAT_MAGIC) {
        const struct fat_header *const fat_header = (const struct fat_header *)header;
        struct fat_arch *arch = (struct fat_arch *)((char *)header + sizeof(struct fat_header));

        for (uint32_t i = 0; i < OSSwapBigToHostInt32(fat_header->nfat_arch); i++, arch++) {
            const struct mach_header *slice_header = (const struct mach_header *)((char *)header + OSSwapBigToHostInt32(arch->offset));

            if (thin_header_looks_legacy(slice_header)) {
                return true;
            }
        }
    }

    return thin_header_looks_legacy(header);
}
