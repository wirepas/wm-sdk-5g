include makefile_common.mk

.DEFAULT_GOAL := all

# Linker script
ifndef MCU_RAM_VAR
LDSCRIPT = $(MCU_PATH)$(MCU_FAMILY)/$(MCU)/linker/gcc_app_$(MCU)$(MCU_SUB)$(MCU_MEM_VAR).ld
else
LDSCRIPT = $(MCU_PATH)$(MCU_FAMILY)/$(MCU)/linker/gcc_app_$(MCU)$(MCU_SUB)$(MCU_MEM_VAR)_$(MCU_RAM_VAR).ld
endif

LIBS :=

ifeq ($(filter $(TARGET_BOARDS),$(target_board)),)
 $(error Board $(target_board) is not in supported board list: ($(TARGET_BOARDS)))
endif

# File to store app version
VERSION_FILE := $(BUILDPREFIX_APP)version.txt

# App different formats
APP_ELF := $(BUILDPREFIX_APP)$(APP_NAME).elf

# For backward compatibility as app makefile except SRCS_PATH variable
SRCS_PATH := $(APP_SRCS_PATH)

# Convert default network settings to CFLAGS to be used in code
ifneq ($(default_network_address),)
CFLAGS += -DNETWORK_ADDRESS=$(default_network_address)
endif
ifneq ($(default_network_channel),)
CFLAGS += -DNETWORK_CHANNEL=$(default_network_channel)
endif
ifneq ($(default_network_cipher_key),)
CFLAGS += -DNET_CIPHER_KEY=$(default_network_cipher_key)
endif
ifneq ($(default_network_authen_key),)
CFLAGS += -DNET_AUTHEN_KEY=$(default_network_authen_key)
endif
# And version numbers
CFLAGS += -DVER_MAJOR=$(app_major) -DVER_MINOR=$(app_minor) -DVER_MAINT=$(app_maintenance) -DVER_DEV=$(app_development)

# Mac profile
ifeq ("$(mac_profile)","ism_24_ghz")
    CFLAGS += -DMAC_PROFILE_ISM24
endif

# Include board init part
-include board/makefile

# Include app specific makefile
-include $(APP_SRCS_PATH)makefile


# Include Libraries config first (dependencies)
-include $(WP_LIB_PATH)config.mk

# Generic util functions are needed for all apps (api.c)
-include $(UTIL_PATH)makefile

# Include libraries code
-include $(WP_LIB_PATH)makefile
INCLUDES += -I$(WP_LIB_PATH)

# Include MCU config first
-include $(MCU_PATH)config.mk

# Include MCU HAL drivers code
-include $(HAL_API_PATH)makefile

# Include common MCU sources
include $(MCU_PATH)common/makefile

#
# Sources & includes paths
#
SRCS += $(APP_SRCS_PATH)app.c
INCLUDES += -I$(API_PATH) -I$(APP_SRCS_PATH)include -I$(UTIL_PATH)

# Objects list
OBJS_ = $(SRCS:.c=.o) $(ASM_SRCS:.s=.o)
OBJS = $(addprefix $(BUILDPREFIX_APP), $(OBJS_))

# Dependent list
DEPS_ = $(SRCS:.c=.d)
DEPS = $(addprefix $(BUILDPREFIX_APP), $(DEPS_))


# Files to be cleaned
CLEAN := $(OBJS) $(APP_ELF) $(APP_HEX) $(DEPS)

$(BUILDPREFIX_APP)%.o : %.c $(APP_SRCS_PATH)makefile $(APP_CONFIG) $(BOARD_CONFIG) $(MCU_CONFIG)
	$(DD)$(MKDIR) $(@D)
	@echo "$(COLOR_CC)CC$(COLOR_END) $<"
	$(D)$(CC) $(INCLUDES) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILDPREFIX_APP)%.o : %.s
	$(DD)$(MKDIR) $(@D)
	@echo "$(COLOR_CC)CC$(COLOR_END) $<"
	$(D)$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@


$(APP_ELF): $(OBJS) $(LIBS)
	@echo "$(COLOR_LINK)Linking$(COLOR_END) $@"
	$(D)$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ \
	      -Wl,-Map=$(BUILDPREFIX_APP)$(APP_NAME).map \
	      -Wl,-T,$(LDSCRIPT),--print-memory-usage $(LIBS)


$(APP_HEX): $(APP_ELF)
	@echo "$(COLOR_DETAILS)CFLAGS are:\n $(CFLAGS)$(COLOR_END)"
	@echo "$(COLOR_DETAILS)LDFLAGS are:\n $(LDFLAGS)$(COLOR_END)"
	@echo "$(COLOR_INFO)Generating:$(COLOR_END) $@"
	$(D)@$(OBJCOPY) $< -O ihex $@

.PHONY: $(VERSION_FILE)
$(VERSION_FILE):
	@echo "app_version=$(app_major).$(app_minor).$(app_maintenance).$(app_development)" > $(@)
	@echo "sha1=$(shell git log -1 --pretty=format:"%h")" >> $(@)

.PHONY: all
all: $(APP_HEX) $(VERSION_FILE)

clean:
	$(D)$(RM) -rf $(CLEAN)

-include $(DEPS)
