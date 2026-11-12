/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

// Modified by Wirepas to use Wirepas I2C driver

#include <stddef.h>
#include "board.h"
#include "i2c.h"
#include "pmic.h"


static struct pmic_conf_t
{
    i2c_conf_t i2c_conf;
    uint8_t i2c_address;
} m_pmic_conf;


pmic_res_e PMIC_configure(uint32_t i2c_clock_freq, bool i2c_pullup, uint8_t i2c_address)
{
    m_pmic_conf.i2c_conf.clock = i2c_clock_freq;
    m_pmic_conf.i2c_conf.pullup = i2c_pullup;
    m_pmic_conf.i2c_address = i2c_address;
    return PMIC_RES_OK;
}


pmic_res_e PMIC_write_reg(uint16_t pmic_address, uint8_t value)
{
    uint8_t buf[] = {
        pmic_address >> 8,
        pmic_address & 0xFF,
        value
    };

    i2c_res_e res = I2C_init(&m_pmic_conf.i2c_conf);
    if (res == I2C_RES_OK || res == I2C_RES_ALREADY_INITIALIZED)
    {
        i2c_xfer_t xfer_tx = {
            .address = m_pmic_conf.i2c_address,
            .write_ptr = buf,
            .write_size = sizeof(buf),
            .read_ptr = NULL,
            .read_size = 0,
            .custom = 0
        };

        res = I2C_transfer(&xfer_tx, NULL);
    }

    I2C_close();
    if (res == I2C_RES_OK)
    {
        return PMIC_RES_OK;
    }
    else
    {
        return PMIC_RES_ERROR;
    }
}


pmic_res_e PMIC_read_reg(uint16_t pmic_address, uint8_t *value)
{
    uint8_t buf[] = {
        pmic_address >> 8,
        pmic_address & 0xFF,
    };

    i2c_res_e res = I2C_init(&m_pmic_conf.i2c_conf);

    if (res == I2C_RES_OK || res == I2C_RES_ALREADY_INITIALIZED)
    {
        i2c_xfer_t xfer_rx = {
            .address = m_pmic_conf.i2c_address,
            .write_ptr = buf,
            .write_size = sizeof(buf),
            .read_ptr = value,
            .read_size  = 1,
            .custom = 0
        };

        res = I2C_transfer(&xfer_rx, NULL);
    }

    I2C_close();

    if (res == I2C_RES_OK)
    {
        return PMIC_RES_OK;
    }
    else
    {
        return PMIC_RES_ERROR;
    }
}
