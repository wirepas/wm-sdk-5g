# Global makefile to build images to be flashed
# or updated through OTAP

include makefile_common.mk

# Version of GCC used for Wirepas testing
GCC_TESTED_VERSION := 10.3.1

# Check the toolchain version with GCC
GCC_VERSION := $(shell $(CC) -dumpversion)
ifneq ($(GCC_VERSION), $(findstring $(GCC_VERSION), $(GCC_TESTED_VERSION)))
$(warning ***********************************************************************)
$(warning "GCC version used is not the recommended and tested by Wirepas )
$(warning "Recommended version is : $(GCC_TESTED_VERSION))
$(warning ***********************************************************************)
endif

#
# Targets
#

# Scratchpads for OTAP

FULL_SCRATCHPAD_NAME := $(APP_NAME)_$(FIRMWARE_NAME)
FULL_SCRATCHPAD_BIN := $(BUILDPREFIX_APP)$(FULL_SCRATCHPAD_NAME).otap
APP_SCRATCHPAD_NAME := $(APP_NAME)
APP_SCRATCHPAD_BIN := $(BUILDPREFIX_APP)$(APP_SCRATCHPAD_NAME).otap
STACK_SCRATCHPAD_NAME := $(FIRMWARE_NAME)
STACK_SCRATCHPAD_BIN := $(BUILDPREFIX_APP)$(STACK_SCRATCHPAD_NAME).otap
ifneq ($(modemfw_area_id),)
ifneq ($(radio),none)
ifneq ($(modem_fw),)
# Name for target to generate a scratchpad with modem + stack + app
FULL_SCRATCHPAD_WITH_MODEMFW_NAME := $(APP_NAME)_$(FIRMWARE_NAME)_modem_fw
FULL_SCRATCHPAD_WITH_MODEMFW_BIN := $(BUILDPREFIX_APP)$(FULL_SCRATCHPAD_WITH_MODEMFW_NAME).otap
# Name for target to generate a scratchpad with modem + stack
STACK_SCRATCHPAD_WITH_MODEMFW_NAME := $(FIRMWARE_NAME)_modem_fw
STACK_SCRATCHPAD_WITH_MODEMFW_BIN := $(BUILDPREFIX_APP)$(STACK_SCRATCHPAD_WITH_MODEMFW_NAME).otap
endif
endif
endif

PLATFORM_CONFIG_INI := $(MCU_PATH)$(MCU_FAMILY)/$(MCU)/ini_files/$(MCU)$(MCU_SUB)$(MCU_MEM_VAR)_platform.ini

CLEAN := $(FULL_SCRATCHPAD_BIN) $(APP_SCRATCHPAD_BIN) $(STACK_SCRATCHPAD_BIN)
CLEAN += $(FULL_SCRATCHPAD_WITH_MODEMFW_BIN) $(STACK_SCRATCHPAD_WITH_MODEMFW_BIN)
CLEAN += $(BUILD_VARIABLES_MK) $(BOOTLOADER_CONFIG_INI)

MODIFIED_STACK_HEX := $(STACK_HEX)

# Final image for programming
FINAL_IMAGE_NAME := final_image_$(APP_NAME)
FINAL_IMAGE_HEX := $(BUILDPREFIX_APP)$(FINAL_IMAGE_NAME).hex

# Hidden file to know if the License was at least displayed one time
# and accepted
LICENSE_ACCEPTED := .license_accepted

CLEAN += $(FINAL_IMAGE_HEX)

# Add targets
TARGETS += $(FINAL_IMAGE_HEX) otap

#
# Manage area id and ini files
#

# Define the app_area as a combination of app_area and HW_VARIANT_ID
ifeq ($(app_specific_area_id),)
$(error You must define a specific area id in your application config.mk file)
endif
app_area_id=$(app_specific_area_id)$(HW_VARIANT_ID)

# Set Default scratchpad ini file if not overriden by app makefile
ifneq ($(INI_FILE),)
$(error You are overriding the default flash partitioning with INI_FILE variable. \
	You must now override INI_FILE_APP variable with a modified version of \
	$(MCU_PATH)$(MCU_FAMILY)/$(MCU)/ini_files/$(MCU)$(MCU_SUB)$(MCU_MEM_VAR)_app.ini)
endif

INI_FILE_WP ?= $(MCU_PATH)$(MCU_FAMILY)/$(MCU)/ini_files/$(MCU)$(MCU_SUB)$(MCU_MEM_VAR)_wp.ini
INI_FILE_APP ?=$(MCU_PATH)$(MCU_FAMILY)/$(MCU)/ini_files/$(MCU)$(MCU_SUB)$(MCU_MEM_VAR)_app.ini

# Default bootloader size is 32 kB, but some architectures support
# other sizes, e.g., 16 kB for legacy network installations
bootloader_size ?= 32k

# By default the key file is positionned at root of SDK and used for all apps/boards
# But it can be overwritten by any app (it will be generated at first execution if needed)
KEY_FILE ?= ./custom_bootloader_keys.ini

#
# Functions
define BUILD_FULL_SCRATCHPAD_WITH_MODEMFW
	@echo "$(COLOR_INFO)Creating Full Scratchpad with modem:$(COLOR_END) $(2) + $(3) + $(4) -> $(1)"
	$(D)$(SCRAT_GEN)    --configfile=$(BOOTLOADER_CONFIG_INI) \
						$(1) \
						$(modemfw_area_id):$(2) \
						$(STACK_CONF):$(stack_area_id):$(3) \
						$(app_major).$(app_minor).$(app_maintenance).$(app_development):$(app_area_id):$(4)
endef

define BUILD_FULL_SCRATCHPAD
	@echo "$(COLOR_INFO)Creating Full Scratchpad:$(COLOR_END) $(2) + $(3) -> $(1)"
	$(D)$(SCRAT_GEN) --configfile=$(BOOTLOADER_CONFIG_INI) \
					$(1) \
					$(STACK_CONF):$(stack_area_id):$(2) \
					$(app_major).$(app_minor).$(app_maintenance).$(app_development):$(app_area_id):$(3)
endef

define BUILD_HEX
	@echo "$(COLOR_INFO)Creating Flashable Hex:$(COLOR_END) $(2) + $(3) + $(4) + $(5) -> $(1)"
	$(D)$(HEX_GEN)  --configfile=$(BOOTLOADER_CONFIG_INI) \
					--bootloader=$(2) \
					$(1) \
					$(STACK_CONF):$(stack_area_id):$(3) \
					$(app_major).$(app_minor).$(app_maintenance).$(app_development):$(app_area_id):$(4) \
					$(5)
endef

define BUILD_APP_SCRATCHPAD
	@echo "$(COLOR_INFO)Creating App Scratchpad:$(COLOR_END) $(2) -> $(1)"
	$(D)$(SCRAT_GEN) --configfile=$(BOOTLOADER_CONFIG_INI) \
					$(1) \
					$(app_major).$(app_minor).$(app_maintenance).$(app_development):$(app_area_id):$(2)
endef

define BUILD_STACK_SCRATCHPAD
	@echo "$(COLOR_INFO)Creating Stack Scratchpad:$(COLOR_END) $(2) -> $(1)"
	$(D)$(SCRAT_GEN) --configfile=$(BOOTLOADER_CONFIG_INI) \
					$(1) \
					$(STACK_CONF):$(stack_area_id):$(2)
endef

define BUILD_STACK_SCRATCHPAD_WITH_MODEMFW
	@echo "$(COLOR_INFO)Creating Stack Scratchpad with modem:$(COLOR_END) $(2) + $(3) -> $(1)"
	$(D)$(SCRAT_GEN)  --configfile=$(BOOTLOADER_CONFIG_INI) \
					$(1) \
					$(modemfw_area_id):$(2) \
					$(STACK_CONF):$(stack_area_id):$(3)
endef

# Params: (1) Final image (2) bootloader (3) Ini file (4) Test app
define BUILD_BOOTLOADER_TEST_APP
	@echo "$(COLOR_INFO)Creating test application for bootloader:$(COLOR_END) $(2) + $(3) + $(4) -> $(1)"
	$(eval output_file:=$(BUILDPREFIX_TEST_BOOTLOADER)temp_file.hex)
	$(HEX_GEN) --configfile=$(3) \
				--bootloader=$(2) \
				$(1) \
				1.0.0.0:$(stack_area_id):$(4)
endef

.PHONY: all app_only otap
all: $(TARGETS)

app_only: $(APP_HEX) $(APP_SCRATCHPAD_BIN)

otap: $(FULL_SCRATCHPAD_BIN) $(APP_SCRATCHPAD_BIN) $(STACK_SCRATCHPAD_BIN) $(FULL_SCRATCHPAD_WITH_MODEMFW_BIN) $(STACK_SCRATCHPAD_WITH_MODEMFW_BIN)

bootloader: $(BOOTLOADER_HEX)

.PHONY: need_board
need_board:
	@# Check if target board is defined
	$(if $(target_board),,$(error No target_board defined.\
	        Please specify one with target_board=<..> argument from command line.\
	        Available boards are: $(AVAILABLE_BOARDS)\
	        A default value can be set in main config.mk file))

	@# Check if board really exist
	@test -s $(BOARD_FOLDER)/config.mk || \
		   { echo "Specified target board $(target_board) doesn't exist. Available boards are: $(AVAILABLE_BOARDS)"; exit 1; }

.PHONY: initial_setup
initial_setup: $(LICENSE_ACCEPTED) $(KEY_FILE)
	@ # Rule to ensure initial setup is done

$(KEY_FILE): | $(LICENSE_ACCEPTED)
	@	# Run the wizard to create key file
	@	# It depends on LICENSE to avoid error when building with -j option
	@	# | (pipe) is intentional to avoid regenerating key file if license is newer
	$(WIZARD) --gen_keys -o $@

$(LICENSE_ACCEPTED):
	@cat LICENSE.txt
	@echo -e "\n\nWirepas SDK is covered by the above License that must be read and accepted.\n\
	For additionnal questions or clarifications, please contact sales@wirepas.com.\n"

	@echo -n -e "\nDo you accept the License Terms? [y/N] " && read ans && [ $${ans:-N} = y ]
	@touch $@

# Add $(STACK_HEX) to PHONY to always call stack makefile
.PHONY: $(STACK_HEX)
$(STACK_HEX): initial_setup need_board $(BUILD_VARIABLES_MK)
	@# Call app makefile to get the hex file of stack
	$(DD)$(MAKE) --no-print-directory -f makefile_stack.mk

# Add $(APP_HEX) to PHONY to always call app makefile
.PHONY: $(APP_HEX)
$(APP_HEX):: initial_setup $(BUILDPREFIX_APP) need_board
	@echo "$(COLOR_INFO)Building $(app_name) for board $(target_board)$(COLOR_END)"
	$(DD)$(MAKE) --no-print-directory -f makefile_app.mk

# Add $(BOOTLOADER_HEX) to PHONY to always call bootloader makefile
.PHONY: $(BOOTLOADER_HEX)
$(BOOTLOADER_HEX): initial_setup need_board $(BUILD_VARIABLES_MK) $(BOOTLOADER_CONFIG_INI)
	@echo "$(COLOR_INFO)Building bootloader$(COLOR_END)"
	$(DD)$(MAKE) --no-print-directory -f makefile_bootloader.mk

.PHONY: $(BOOTLOADER_TEST_HEX)
$(BOOTLOADER_TEST_HEX): initial_setup need_board $(BOOTLOADER_CONFIG_INI)
	@# Call bootloader test makefile to get the test application hex file
	$(DD)$(MAKE) --no-print-directory -f makefile_bootloader_test.mk


$(STACK_SCRATCHPAD_BIN): initial_setup $(MODIFIED_STACK_HEX) $(BOOTLOADER_CONFIG_INI)
	$(call BUILD_STACK_SCRATCHPAD,$(STACK_SCRATCHPAD_BIN),$(MODIFIED_STACK_HEX))

$(APP_SCRATCHPAD_BIN): initial_setup $(APP_HEX) $(BOOTLOADER_CONFIG_INI)
	$(call BUILD_APP_SCRATCHPAD,$(APP_SCRATCHPAD_BIN),$(APP_HEX))

$(FULL_SCRATCHPAD_BIN): initial_setup $(MODIFIED_STACK_HEX) $(APP_HEX) $(BOOTLOADER_CONFIG_INI)
	$(call BUILD_FULL_SCRATCHPAD,$(FULL_SCRATCHPAD_BIN),$(MODIFIED_STACK_HEX),$(APP_HEX))

ifneq ($(modemfw_area_id),)
ifneq ($(radio),none)
$(FULL_SCRATCHPAD_WITH_MODEMFW_BIN): initial_setup $(MODIFIED_STACK_HEX) $(APP_HEX) $(BOOTLOADER_CONFIG_INI)
	$(call BUILD_FULL_SCRATCHPAD_WITH_MODEMFW,$(FULL_SCRATCHPAD_WITH_MODEMFW_BIN),${modem_fw},$(MODIFIED_STACK_HEX),$(APP_HEX))

$(STACK_SCRATCHPAD_WITH_MODEMFW_BIN): initial_setup $(MODIFIED_STACK_HEX) $(APP_HEX) $(BOOTLOADER_CONFIG_INI)
	$(call BUILD_STACK_SCRATCHPAD_WITH_MODEMFW,$(STACK_SCRATCHPAD_WITH_MODEMFW_BIN),${modem_fw},$(MODIFIED_STACK_HEX))

endif
endif

# Final image always uses STACK_HEX instead of MODIFIED_STACK_HEX,
# as the included bootloader is already up to date
$(FINAL_IMAGE_HEX): initial_setup $(STACK_HEX) $(APP_HEX) $(BOOTLOADER_HEX) $(BOOTLOADER_CONFIG_INI)
	$(call BUILD_HEX,$(FINAL_IMAGE_HEX),$(BOOTLOADER_HEX),$(STACK_HEX),$(APP_HEX),$(EXTRA_HEX))

$(BOOTLOADER_CONFIG_INI): initial_setup $(BUILDPREFIX_APP) $(INI_FILE_WP) $(INI_FILE_APP)
	@	# Rule to create the full config file based on multiple ini files and store it per build folder
	$(D)$(BOOT_CONF)    -i $(INI_FILE_WP) -i $(INI_FILE_APP) -i $(KEY_FILE) -i $(PLATFORM_CONFIG_INI) \
						-o $@ \
						-ol APP_AREA_ID:$(app_area_id) \
						-hm 0x$(HW_MAGIC) \
						-hv 0x$(HW_VARIANT_ID) \
						-fb $(FLASH_BASE_ADDR) \
						-bs $(bootloader_size) \
						-as $(BOOTLOADER_SIZES)

# Generate and include a makefile fragment for build-time
# make variables that depend on selected configuration
$(BUILD_VARIABLES_MK): $(BUILDPREFIX_APP)
	$(D)$(BOOT_CONF)    -i $(INI_FILE_WP) -i $(INI_FILE_APP) -i $(PLATFORM_CONFIG_INI) \
						-om $@ \
						-ol APP_AREA_ID:$(app_area_id) \
						-hm 0x$(HW_MAGIC) \
						-hv 0x$(HW_VARIANT_ID) \
						-fb $(FLASH_BASE_ADDR) \
						-bs $(bootloader_size) \
						-as $(BOOTLOADER_SIZES)

-include $(BUILD_VARIABLES_MK)

bootloader_test: $(BOOTLOADER_HEX) $(BOOTLOADER_TEST_HEX) $(BOOTLOADER_CONFIG_INI)
	$(call BUILD_BOOTLOADER_TEST_APP,$(BUILDPREFIX_APP)final_bootloader_test.hex,$<,$(BOOTLOADER_CONFIG_INI),$(word 2,$^))

$(BUILDPREFIX_APP):
	$(DD)$(MKDIR) $@

.PHONY: doxygen
doxygen:
	@	# If build folder does not exist, create it
	$(DD)mkdir -p $(GLOBAL_BUILD)
	doxygen projects/doxygen/Doxyfile.template
	@	# Replace search engine
	cp -rf projects/doxygen/search.js $(GLOBAL_BUILD)html/search/search.js
	$(RM) $(GLOBAL_BUILD)html/search/search*.png
	echo "<script id=\"searchdata\" type=\"text/xmldata\">" >> $(GLOBAL_BUILD)html/search.html
	cat searchdata.xml >> $(GLOBAL_BUILD)html/search.html
	echo "</script>" >> $(GLOBAL_BUILD)html/search.html
	$(RM) searchdata.xml

# clean the specified app
.PHONY: clean
clean: need_board
	@echo "$(COLOR_INFO)Cleaning for $(app_name) and board $(target_board)$(COLOR_END)"
	$(D)$(RM) -rf $(BUILDPREFIX_APP)

# clean all the apps
.PHONY: clean_all
clean_all:
	@echo "$(COLOR_INFO)Cleaning everything$(COLOR_END)"
	$(D)$(RM) -rf $(GLOBAL_BUILD)
