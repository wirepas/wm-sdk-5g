/* Copyright 2024 Wirepas Ltd. All Rights Reserved.
 *
 * See file LICENSE.txt for full license details.
 *
 */

#include <string.h>
#include "board.h"
#include "mcu.h"
#include "external_flash.h"
#include "external_flash_gd25wb256e.h"
#include "nrf_gpio.h"
#include "nrf_spim.h"

#define MIN(x,y)    (x < y? x : y)

// https://www.gigadevice.com.cn/Public/Uploads/uploadfile/files/20220714/DS-00863-GD25WB256E-Rev1.1.pdf
// page 48-49:
static const flash_info_t m_flash_info = {
    .flash_size = 32UL * 1024UL * 1024UL,  // 256-mbit flash
    .write_page_size = 256,
    .erase_sector_size = 32UL * 1024UL,   // 32kB
    .write_alignment = 1,
    .byte_write_time = 150,               // Typical 80 us, Max 150 us. (first byte)
    .page_write_time = 4000,              // Typical 500 us, Max 4 ms.
    .sector_erase_time = 2000000,         // Typical 250 ms, Max 2 s.
    .byte_write_call_time = 94,           // TODO: isBusy time + 4+1 bytes transfered on SPI bus + 20uS code exec time.
    .page_write_call_time = 2134,         // TODO: isBusy time + 4+256 bytes transfered on SPI bus + 20uS code exec time.
    .sector_erase_call_time = 78,         // TODO: isBusy time + 3 bytes transfered on SPI bus + 20uS code exec time.
    .is_busy_call_time = 44               // TODO: 3 bytes transfered on SPI bus + 20uS code exec time.
};

//-----------------------------------------------------------------------------
#if defined(EXT_FLASH_DRIVER_DEBUG_LED)

static const uint8_t gpio_pin_map[] = BOARD_GPIO_PIN_LIST;
static const uint8_t led_id_map[] = BOARD_LED_ID_LIST;

void ext_flash_debug_init(void)
{
    uint8_t led_id, gpio_pin;
    // Configure LED GPIOs to output and switch LEDs off
    for(led_id = 0; led_id < sizeof(led_id_map); led_id++)
    {
        gpio_pin = gpio_pin_map[led_id_map[led_id]];
        nrf_gpio_pin_dir_set(gpio_pin, NRF_GPIO_PIN_DIR_OUTPUT);
        nrf_gpio_pin_write(gpio_pin, false);
    }
}


void ext_flash_debug_success(void)
{
    // one LED = OK
    nrf_gpio_pin_write(gpio_pin_map[led_id_map[1]], true);
    nrf_gpio_pin_write(gpio_pin_map[led_id_map[0]], false);
    nrf_gpio_pin_write(gpio_pin_map[led_id_map[2]], false);
}


void ext_flash_debug_fail(void)
{
    // two LEDs = error
    nrf_gpio_pin_write(gpio_pin_map[led_id_map[1]], false);
    nrf_gpio_pin_write(gpio_pin_map[led_id_map[0]], true);
    nrf_gpio_pin_write(gpio_pin_map[led_id_map[2]], true);
}


#else // defined(EXT_FLASH_DRIVER_DEBUG_LED)

void ext_flash_debug_init(void){}
void ext_flash_debug_success(void){}
void ext_flash_debug_fail(void){}

#endif // defined(EXT_FLASH_DRIVER_DEBUG_LED)
//-----------------------------------------------------------------------------


static inline void set_address(uint8_t * command_p, uint32_t address)
{
    *command_p++ = (address >> 16) & 0xFF;
    *command_p++ = (address >> 8) & 0xFF;
    *command_p   = address & 0xFF;
}


static void ext_flash_select(bool select)
{
    nrf_gpio_pin_write(EXT_FLASH_CS, !select);
}


static void ext_flash_spi_xfer(const uint8_t *pTx, int txSize, uint8_t *pRx, int rxSize, bool async) {
    nrf_spim_event_clear(EXT_FLASH_SPIM_P, NRF_SPIM_EVENT_END);
    nrf_spim_tx_buffer_set(EXT_FLASH_SPIM_P, pTx, txSize);
    nrf_spim_rx_buffer_set(EXT_FLASH_SPIM_P, pRx, rxSize);
    nrf_spim_task_trigger(EXT_FLASH_SPIM_P, NRF_SPIM_TASK_START);
    if (!async) {
        // Wait for SPI transfer to be finished
        while (!nrf_spim_event_check(EXT_FLASH_SPIM_P, NRF_SPIM_EVENT_END));
        nrf_spim_event_clear(EXT_FLASH_SPIM_P, NRF_SPIM_EVENT_END);
    }
}


static bool ext_flash_write_enable()
{
    uint8_t cmd[1], reply[2];

    cmd[0] = GD25WB256E_CMD_WRITE_ENABLE;

    ext_flash_select(true);
    ext_flash_spi_xfer(cmd, sizeof(cmd), NULL, 0, false);
    ext_flash_select(false);

    // Wait while WIP is set
    while(externalFlash_isBusy());

    cmd[0] = GD25WB256E_CMD_READ_STATUS1;
    ext_flash_select(true);
    ext_flash_spi_xfer(cmd, sizeof(cmd), reply, sizeof(reply), false);
    ext_flash_select(false);
    return (reply[1] & GD25WB256E_STATUS_WEL) == GD25WB256E_STATUS_WEL;
}


#if defined(EXT_FLASH_CONTENT_ALTERING_TEST)

static bool ext_flash_test_erase()
{
    size_t addr = 0;
    size_t sectors = 1;
    uint8_t buff[16];

    // Make sure flash is ready
    while(externalFlash_isBusy());

    // Erase first sector
    if (externalFlash_startErase(&addr, &sectors) != EXTFLASH_RES_OK)
    {
        return false;
    }

    // Make sure flash is ready
    while(externalFlash_isBusy());

    if (sectors != 0)
    {
        return false;
    }
    if (addr != 32768)
    {
        return false;
    }

    // Read that first 16 bytes and check those are erased
    if (externalFlash_startRead(buff, 0, 16) != EXTFLASH_RES_OK)
    {
        return false;
    }
    for(uint8_t t=0; t<16; t++)
    {
        if (buff[t] != 0xff)
        {
            return false;
        }
    }

    return true;
}


static bool ext_flash_test_write()
{
    uint8_t buff[16];

    // Create test data to buffer
    for(uint8_t t=0; t<16; t++)
    {
        buff[t] = t ^ 0xaa;
    }

    // Write test data to beginning of external flash
    if (externalFlash_startWrite(0, buff, 16) != EXTFLASH_RES_OK)
    {
        return false;
    }

    // Make sure flash is ready
    while(externalFlash_isBusy());

    // Read that first 16 bytes and check those are the same as test data
    if (externalFlash_startRead(buff, 0, 16) != EXTFLASH_RES_OK)
    {
        return false;
    }
    for(uint8_t t=0; t<16; t++)
    {
        if (buff[t] != (t ^ 0xaa))
        {
            return false;
        }
    }
    return true;
}


static bool ext_flash_test()
{
    return (ext_flash_test_erase() && ext_flash_test_write());
}

#else

static bool ext_flash_test()
{
    return true;
}

#endif


extFlash_res_e externalFlash_init(void)
{
    // CS up as soon as possible
    nrf_gpio_pin_dir_set(EXT_FLASH_CS, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_write(EXT_FLASH_CS, true);

    ext_flash_debug_init();

    nrf_gpio_pin_dir_set(EXT_FLASH_SPI_SCK, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_dir_set(EXT_FLASH_SPI_MOSI, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_dir_set(EXT_FLASH_SPI_MISO, NRF_GPIO_PIN_DIR_INPUT);

    nrf_gpio_cfg(EXT_FLASH_SPI_SCK,
                 NRF_GPIO_PIN_DIR_OUTPUT,
                 NRF_GPIO_PIN_INPUT_CONNECT,
                 NRF_GPIO_PIN_NOPULL,
                 NRF_GPIO_PIN_S0S1,
                 NRF_GPIO_PIN_NOSENSE);

    nrf_spim_pins_set(EXT_FLASH_SPIM_P, EXT_FLASH_SPI_SCK, EXT_FLASH_SPI_MOSI, EXT_FLASH_SPI_MISO);
    nrf_spim_frequency_set(EXT_FLASH_SPIM_P, NRF_SPIM_FREQ_8M);
    /* Both GD25WB256E (DK) and MX25R6435F (Thingy:X) support only SPI modes 0 and 3. */
    nrf_spim_configure(EXT_FLASH_SPIM_P, NRF_SPIM_MODE_3, NRF_SPIM_BIT_ORDER_MSB_FIRST);

    nrf_spim_orc_set(EXT_FLASH_SPIM_P, 0);
    nrf_spim_enable(EXT_FLASH_SPIM_P);

    uint8_t cmd[1], reply[4];

    // Wait until nRF9161 has access to external flash.
    // Thus polling external flash in a loop here:
    for(uint32_t t=0; t<1000; t++)
    {
        cmd[0] = GD25WB256E_CMD_READ_IDENTIFICATION;

        ext_flash_select(true);
        ext_flash_spi_xfer(cmd, sizeof(cmd), reply, sizeof(reply), false);
        ext_flash_select(false);

        if ((reply[1] == GD25WB256E_ID_1) &&
            (reply[2] == GD25WB256E_ID_2) &&
            (reply[3] == GD25WB256E_ID_3))
        {
            // Flash chip is detected
            // Test that flash works as expected
            if (ext_flash_test())
            {
                // Everything ok
                // Initialization completed
                ext_flash_debug_success();
                return EXTFLASH_RES_OK;
            }
            // Flash chip was detected, but did not work as expected
            // Bail out from detection loop
            break;
        }
    }
    ext_flash_debug_fail();
    return EXTFLASH_RES_ERROR;
}


extFlash_res_e externalFlash_startRead(void * to, const void * from, size_t amount)
{
    uint8_t cmd[4];
    uint8_t * buf = (uint8_t *)to;
    uint32_t address = (uint32_t)from;

    if (externalFlash_isBusy())
    {
        return EXTFLASH_RES_BUSY;
    }

    if ((address + amount) > m_flash_info.flash_size)
    {
        return EXTFLASH_RES_PARAM;
    }

    cmd[0] = GD25WB256E_CMD_READ_ARRAY;
    set_address(&cmd[1], address);

    ext_flash_select(true);
    ext_flash_spi_xfer(cmd, sizeof(cmd), NULL, 0, false);

    while (amount > 0) {
        uint32_t bytesToRead = MIN(amount, GD25WB256E_MAX_TRANSFER_SIZE);

        ext_flash_spi_xfer(NULL, 0, buf, bytesToRead, false);
        amount -= bytesToRead;
        buf += bytesToRead;
    }
    ext_flash_select(false);

    return EXTFLASH_RES_OK;
}


extFlash_res_e externalFlash_startWrite(void * to, const void * from, size_t amount)
{
    uint8_t cmd[4];
    uint32_t address = (uint32_t)to;
    uint8_t * buf = (uint8_t *)from;

    if (externalFlash_isBusy())
    {
        return EXTFLASH_RES_BUSY;
    }

    /* Check that write do not cross page boundary */
    uint32_t next_page = (address &
                          (0xFFFFFFFF - (m_flash_info.write_page_size - 1))) +
                         m_flash_info.write_page_size;

    if((address + amount) > next_page)
    {
        return EXTFLASH_RES_PARAM;
    }

    if ((address + amount) > m_flash_info.flash_size)
    {
        return EXTFLASH_RES_PARAM;
    }

    if (amount > m_flash_info.write_page_size)
    {
        return EXTFLASH_RES_PARAM;
    }

    if (!ext_flash_write_enable())
    {
        return EXTFLASH_RES_ERROR;
    }

    cmd[0] = GD25WB256E_CMD_PROGRAM_PAGE;
    set_address(&cmd[1], address);

    ext_flash_select(true);
    ext_flash_spi_xfer(cmd, sizeof(cmd), NULL, 0, false);

    while (amount > 0)
    {
        uint32_t bytesToWrite = MIN(amount, GD25WB256E_MAX_TRANSFER_SIZE);

        ext_flash_spi_xfer(buf, bytesToWrite, NULL, 0, false);
        amount -= bytesToWrite;
        buf += bytesToWrite;
    }
    ext_flash_select(false);

    return EXTFLASH_RES_OK;
}


bool externalFlash_isBusy(void)
{
    uint8_t cmd[1], reply[2];

    cmd[0] = GD25WB256E_CMD_READ_STATUS1;
    ext_flash_select(true);
    ext_flash_spi_xfer(cmd, sizeof(cmd), reply, sizeof(reply), false);
    ext_flash_select(false);
    return (reply[1] & GD25WB256E_STATUS_WIP) == GD25WB256E_STATUS_WIP;
}


extFlash_res_e externalFlash_startErase(size_t * sector_base, size_t * number_of_sector)
{
    uint8_t cmd[4];
    uint32_t address = (uint32_t)*sector_base;

    if (externalFlash_isBusy())
    {
        return EXTFLASH_RES_BUSY;
    }

    if ((address + m_flash_info.erase_sector_size) > m_flash_info.flash_size)
    {
        return EXTFLASH_RES_PARAM;
    }

    if ((address % m_flash_info.erase_sector_size) != 0)
    {
        return EXTFLASH_RES_PARAM;
    }

    if (!ext_flash_write_enable())
    {
        return EXTFLASH_RES_ERROR;
    }

    cmd[0] = GD25WB256E_CMD_BLOCK_ERASE_32K;
    set_address(&cmd[1], address);

    ext_flash_select(true);
    ext_flash_spi_xfer(cmd, sizeof(cmd), NULL, 0, false);
    ext_flash_select(false);

    *sector_base += m_flash_info.erase_sector_size;
    *number_of_sector -= 1;

    return EXTFLASH_RES_OK;
}


extFlash_res_e externalFlash_getInfo(flash_info_t * info)
{
    memcpy(info,&m_flash_info,sizeof(flash_info_t));

    return EXTFLASH_RES_OK;
}
