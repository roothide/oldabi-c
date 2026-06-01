//
// Copyright (c) 2026 Nightwind
//

#include "utils.h"

__attribute__((visibility("hidden")))
bool string_has_prefix(const char *_Nonnull const string, const char *_Nonnull const prefix) {
	const size_t string_length = strlen(string);
	const size_t prefix_length = strlen(prefix);

	if (string_length < prefix_length) {
		return false;
	}

	return strncmp(string, prefix, prefix_length) == 0;
}
