
MCU_COMMON_SRCS_PATH := $(MCU_PATH)common/

include $(MCU_COMMON_SRCS_PATH)radio/makefile

ASM_SRCS += $(MCU_COMMON_SRCS_PATH)entrypoint.s
SRCS += $(MCU_COMMON_SRCS_PATH)start.c \
        $(MCU_COMMON_SRCS_PATH)ds.c

ifeq ($(HAL_BUTTON), yes)
SRCS += $(MCU_COMMON_SRCS_PATH)button.c
endif

ifeq ($(HAL_LED), yes)
SRCS += $(MCU_COMMON_SRCS_PATH)led.c
endif

ifeq ($(HAL_GPIO), yes)
SRCS += $(MCU_COMMON_SRCS_PATH)gpio_weak.c
endif

# Define DECT Band Group
# Check if default_dect_band_group is provided via command line
ifneq ($(origin default_dect_band_group),undefined)
    CFLAGS += -DDEFAULT_DECT_BAND_GROUP=$(default_dect_band_group) -DDECT_BAND_CONFIG
endif

