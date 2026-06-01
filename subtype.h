//
// Copyright (c) 2026 Nightwind
//

#ifndef SUBTYPE_H
#define SUBTYPE_H

#include <mach-o/dyld.h>
#include <mach-o/fat.h>

bool header_looks_legacy(const struct mach_header *_Nonnull const header);

#endif // SUBTYPE_H
