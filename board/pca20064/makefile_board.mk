SRCS += board/$(target_board)/board_custom_init.c

SRCS += board/$(target_board)/npm1300_init.c
SRCS += mcu/nrf/nrf91/hal/pmic.c
SRCS += mcu/nrf/common/hal/i2c.c
INCLUDES += -Imcu/hal_api
INCLUDES += -Ibootloader
INCLUDES += -Imcu/nrf/nrf91/hal
