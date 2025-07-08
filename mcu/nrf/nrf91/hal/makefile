NRF_FAMILY_HAL_PREFIX := $(MCU_PATH)$(MCU_FAMILY)/common/hal/
NRF91_HAL_PREFIX := $(MCU_PREFIX)hal/

# Add include path for nrf91 specific HAL header files
INCLUDES += -I$(NRF91_HAL_PREFIX)

# Build nrf91 specific HAL drivers
ifeq ($(HAL_UART), yes)
# Check version
ifeq ($(UART_USE_USB), yes)
        $(error Board $(target_board) doesn't support USB currently)
else
        SRCS += $(NRF_FAMILY_HAL_PREFIX)usart_dma.c
endif
endif

SRCS += $(NRF91_HAL_PREFIX)power.c
