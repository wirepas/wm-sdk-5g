/* Copyright 2024 Wirepas Ltd. All Rights Reserved.
 *
 * See file LICENSE.txt for full license details.
 *
 */

/**
 * @file
 * The public board description for nRF9131-EK board was not available
 * from Nordic Semiconductor TechDocs pages at the time writing this
 * board definition. Please search nRF9131-EK board definition from
 * <a href="https://docs.nordicsemi.com/category/nrf-91-series"</a>'
 */
#ifndef BOARD_PCA10165_BOARD_H_
#define BOARD_PCA10165_BOARD_H_


// NRF_GPIO is mapped to NRF_P0 , for pins P0.00 ... P0.31
// With nrf_gpio.h, use SW_pin (logical pins, port-aware)

/**
NRF_P0  SW_pin  PCA10165                Notes (recommended usage)
------------------------------------------------------------------------
P0.00    0      gpio
P0.01    1      gpio
P0.02    2      gpio
P0.03    3      gpio
P0.04    4      gpio
P0.05    5      gpio
P0.06    6      gpio
P0.07    7      SCL                     I2C/npm1300
P0.08    8      SDA                     I2C/npm1300
P0.09    9      gpio/UART1_CTS
P0.10   10      gpio/UART1_RTS
P0.11   11      gpio/UART1_TX
P0.12   12      gpio/UART1_RX
P0.13   13      gpio/AIN0
P0.14   14      gpio/AIN1
P0.15   15      gpio/AIN2
P0.16   16      gpio/AIN3
P0.17   17      gpio/AIN4
P0.18   18      gpio/AIN5
P0.19   19      gpio/AIN6
P0.20   20      SCK/AIN7               external flash memory SCK
P0.21   21      MOSI/TRACECLK          external flash memory MOSI/TRACE port Clock
P0.22   22      MISO/TRACEDATA0        external flash memory MISO/TRACE port data 0
P0.23   23      UART2_RX/TRACEDATA1    UART2 RX/TRACE port data 1
P0.24   24      UART2_TX/TRACEDATA2    UART2 TX/TRACE port data 2
P0.25   25      UART2_RTS/TRACEDATA3   UART2 RTS/TRACE port data 3
P0.26   26      CS                     external flash memory CS
P0.27   27      UART2_CTS              UART2 CTS
P0.28   28      gpio/BUTTON(SW1)
P0.29   29      gpio/LED-R
P0.30   30      gpio/LED-G
P0.31   31      gpio/LED-B
*/

// Serial port pins for UART1
#define BOARD_USART_TX_PIN              11
#define BOARD_USART_RX_PIN              12
#define BOARD_USART_CTS_PIN              9  /* For USE_USART_HW_FLOW_CONTROL */
#define BOARD_USART_RTS_PIN             10  /* For USE_USART_HW_FLOW_CONTROL */


#define BOARD_GPIO_PIN_LIST            {29,  /* P0.29*/\
                                        30,  /* P0.30*/\
                                        31,  /* P0.31*/\
                                        28,  /* P0.28 BUTTON */\
                                        12,  /* P0.12 required by the dual_mcu app. usart wakeup pin (= BOARD_USART_RX) */\
                                        13}  /* P0.13 required by the dual_mcu app (indication signal) */


// User friendly name for GPIOs (IDs mapped to the BOARD_GPIO_PIN_LIST table)
#define BOARD_GPIO_ID_LED1               0  // mapped to pin P0.29
#define BOARD_GPIO_ID_LED2               1  // mapped to pin P0.30
#define BOARD_GPIO_ID_LED3               2  // mapped to pin P0.31
#define BOARD_GPIO_ID_BUTTON1            3  // mapped to pin P0.28

#define BOARD_GPIO_ID_USART_WAKEUP       4  // mapped to pin P0.12
#define BOARD_GPIO_ID_UART_IRQ           5  // mapped to pin P0.13

// List of LED IDs
#define BOARD_LED_ID_LIST              {BOARD_GPIO_ID_LED1, BOARD_GPIO_ID_LED2, BOARD_GPIO_ID_LED3}

// List of button IDs mapped to GPIO IDs
#define BOARD_BUTTON_ID_LIST           {BOARD_GPIO_ID_BUTTON1}

// Active low polarity for LEDs
#define BOARD_LED_ACTIVE_LOW            false

// Active low polarity for buttons
#define BOARD_BUTTON_ACTIVE_LOW         true

// Active internal pull-up for buttons
#define BOARD_BUTTON_INTERNAL_PULL      true

// The board supports DCDC (#define BOARD_SUPPORT_DCDC)
// Since SDK v1.2 (bootloader > v7) this option has been move to
// board/<board_name>/config.mk. Set board_hw_dcdc to yes to enable DCDC.
#ifdef BOARD_SUPPORT_DCDC
#error This option has been moved to board/<board_name>/config.mk
#endif

// External Flasn Memory
#define EXT_FLASH_SPI_MOSI             21 // P0.21
#define EXT_FLASH_SPI_MISO             22 // P0.22
#define EXT_FLASH_SPI_SCK              20 // P0.20
#define EXT_FLASH_CS                   26 // P0.26
#define EXT_FLASH_SPIM_P               NRF_SPIM1
// Enable external flash memory debugging using LEDs.
#define EXT_FLASH_DRIVER_DEBUG_LED


/**
 * I2C
 */

#define USE_I2C2

// I2C Port pin
#define BOARD_I2C_SCL_PIN              7 // P0.07
#define BOARD_I2C_SDA_PIN              8 // P0.08
#define BOARD_I2C_PIN_PULLUP           true





#endif /* BOARD_PCA10153_BOARD_H_ */
