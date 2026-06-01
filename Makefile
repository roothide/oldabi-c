TARGET := iphone:clang:latest:15.0
ARCHS = arm64e

INSTALL_TARGET_PROCESSES = SpringBoard

include $(THEOS)/makefiles/common.mk

TWEAK_NAME = oldabi-c

oldabi-c_FILES = tweak.c patchfind.c subtype.c utils.c
oldabi-c_FRAMEWORKS = CydiaSubstrate
oldabi-c_CFLAGS = -Werror -Wpedantic -Wno-nullability-extension -Wno-int-conversion
oldabi-c_INSTALL = 0

include $(THEOS_MAKE_PATH)/tweak.mk

internal-stage::
	$(ECHO_NOTHING)mkdir -p $(THEOS_STAGING_DIR)/usr/lib/ellekit/$(ECHO_END)
	$(ECHO_NOTHING)cp $(THEOS_OBJ_DIR)/oldabi-c.dylib $(THEOS_STAGING_DIR)/usr/lib/ellekit/OldABI.dylib$(ECHO_END)