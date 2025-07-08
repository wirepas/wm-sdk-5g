/**
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * Modified by Wirepas to use Wirepas I2C driver
 * Refereces:
 *  [1] https://infocenter.nordicsemi.com/pdf/nPM1300_PS_v1.0.pdf
 */

#include <stddef.h>
#include "board.h"
#include "pmic.h"
#include "npm1300_init.h"

// I2C address of power management chip
#define PM1300_I2C_ADDRESS   0x6B
#define PM1300_CLOCK_FREQ_HZ 100000

// I2C address of wifi power management chip
#define PM6001_I2C_ADDRESS 0x70

#define CHECKERR                                                               \
    if (err)                                                                   \
    {                                                                          \
        return err;                                                            \
    }


static int power_mgmt_init(void)
{
    pmic_res_e err = 0;

    // Configure PMIC before using it
    err = PMIC_configure(PM1300_CLOCK_FREQ_HZ,
                         BOARD_I2C_PIN_PULLUP,
                         PM1300_I2C_ADDRESS);
    CHECKERR;

    // Select value and TRIM to match Battery NTC resistance, see [1] 7.1.10.9
    err = PMIC_write_reg(0x050A, 0x01);
    CHECKERR;
    // Battery Charger Normal termination voltage, see [1] 6.2.13.12
    err = PMIC_write_reg(0x030C, 0x07);
    CHECKERR;
    // Battery Charger Warm termination voltage, see [1] 6.2.13.13
    err = PMIC_write_reg(0x030D, 0x04);
    CHECKERR;
    // Battery Charger current setting, see [1] 6.2.13.8
    err = PMIC_write_reg(0x0308, 0x25);
    CHECKERR;
    // Battery Charger current setting, see [1] 6.2.13.9
    err = PMIC_write_reg(0x0309, 0x00);
    CHECKERR;
    // Battery Charger discharge current limit, see [1] 6.2.13.10
    err = PMIC_write_reg(0x030A, 0x9A);
    CHECKERR;
    // Battery Charger discharge current limit, see [1] 6.2.13.11
    err = PMIC_write_reg(0x030B, 0x01);
    CHECKERR;
    // VBUS current limit, see [1] 6.1.8 (undocumented)
    err = PMIC_write_reg(0x0202, 0x05);
    CHECKERR;
    // Battery Charger Vtrickle select, see [1] 6.2.13.14
    err = PMIC_write_reg(0x030E, 0x00);
    CHECKERR;
    // Battery Charger ITERM select, see [1] 6.2.13.15
    err = PMIC_write_reg(0x030F, 0x00);
    CHECKERR;
    // Enable Auto IBAT measurement after VBAT task, see [1] 7.1.10.24
    err = PMIC_write_reg(0x0524, 0x01);
    CHECKERR;
    // Start VBAT Measurement, see [1] 7.1.10.1
    err = PMIC_write_reg(0x0500, 0x01);
    CHECKERR;
    // Start Battery NTC thermistor Measurement, see [1] 7.1.10.2
    err = PMIC_write_reg(0x0501, 0x01);
    CHECKERR;
    // Start Die Temperature Measurement, see [1] 7.1.10.3
    err = PMIC_write_reg(0x0502, 0x01);
    CHECKERR;
    // enable automatic thermistor and die temperature monitoring, see
    // [1] 7.1.10.11
    err = PMIC_write_reg(0x050C, 0x01);
    CHECKERR;
    // Enable battery charger, see [1] 6.2.13.4
    err = PMIC_write_reg(0x0304, 0x01);
    CHECKERR;

    return err;
}


int npm1300_init(void)
{
    int err;

    err = power_mgmt_init();
    if (err)
    {
        return err;
    }

    return 0;
}
