/* Copyright 2024 Wirepas Ltd. All Rights Reserved.
 *
 * See file LICENSE.txt for full license details.
 *
 */
#ifndef EXTERNAL_FLASH_GD25LE255E_H_
#define EXTERNAL_FLASH_GD25LE255E_H_

// GD25LE255E chip identification
#define GD25LE255E_ID_1 0xC8 //M7-M0
#define GD25LE255E_ID_2 0x60 //ID23-ID16
#define GD25LE255E_ID_3 0x19 //ID15-ID8
#define GD25LE255E_ID_4 0xff //ID7-ID0
// GD25LE255E Status register No.1.
typedef enum
{
    GD25LE255E_STATUS_WIP = 0x01, // Write In Progress = Busy
    GD25LE255E_STATUS_WEL = 0x02, // Write Enable Latch
} ext_flash_status1_t;

// GD25LE255E Maximum transfer size.
// Limited by Nordic SPI master to 255 bytes per SPI DMA transaction.
#define GD25LE255E_MAX_TRANSFER_SIZE 0xff

// SPI commands
enum {
    GD25LE255E_CMD_WRITE_STATUS1       = 0x01,
    GD25LE255E_CMD_PROGRAM_PAGE        = 0x02,
    GD25LE255E_CMD_READ_ARRAY          = 0x03,
    GD25LE255E_CMD_WRITE_DISABLE       = 0x04,
    GD25LE255E_CMD_READ_STATUS1        = 0x05,
    GD25LE255E_CMD_WRITE_ENABLE        = 0x06,
    GD25LE255E_CMD_SECTOR_ERASE        = 0x20,
    GD25LE255E_CMD_BLOCK_ERASE_32K     = 0x52,
    GD25LE255E_CMD_READ_STATUS2        = 0x35,
    GD25LE255E_CMD_READ_IDENTIFICATION = 0x9F,
} ext_flash_cmd_t;

#endif // EXTERNAL_FLASH_GD25LE255E_H_
