/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

// Modified by Wirepas to use Wirepas I2C driver

#include <stddef.h>
#include "board.h"
#include "pmic.h"
#include "npm1300_init.h"

// I2C address of power management chip
#define PM1300_I2C_ADDRESS          0x6B
#define PM1300_CLOCK_FREQ_HZ 100000

// I2C address of wifi power management chip
#define PM6001_I2C_ADDRESS          0x70

#define CHECKERR if (err) { return err;}


static int power_mgmt_init(void)
{
    pmic_res_e err = 0;
    uint8_t reg = 0;
    // Configure PMIC before using it
    err = PMIC_configure(PM1300_CLOCK_FREQ_HZ, BOARD_I2C_PIN_PULLUP, PM1300_I2C_ADDRESS);
    CHECKERR;

    // disable charger for config
    err = PMIC_write_reg(0x0305, 0x03); CHECKERR;

    // set VBUS current limit 500mA
    err = PMIC_write_reg(0x0201, 0x00); CHECKERR;
    err = PMIC_write_reg(0x0202, 0x00); CHECKERR;
    err = PMIC_write_reg(0x0200, 0x01); CHECKERR;

    // set RF switch to BLE by default
    err = PMIC_write_reg(0x0601, 0x08); CHECKERR;

    // enable VDD_SENS:
    err = PMIC_write_reg(0x0802, 0x01); CHECKERR;

    // let BUCK2 be controlled by GPIO2
    err = PMIC_write_reg(0x0602, 0x00); CHECKERR;
    err = PMIC_write_reg(0x040C, 0x18); CHECKERR;

    // set bias resistor for 10k NTC
    err = PMIC_write_reg(0x050A, 0x01); CHECKERR;
    // set COLD threshold to 0C
    err = PMIC_write_reg(0x0310, 0xbb); CHECKERR;
    err = PMIC_write_reg(0x0311, 0x01); CHECKERR;
    // set COOL threshold to 10C
    err = PMIC_write_reg(0x0312, 0xa4); CHECKERR;
    err = PMIC_write_reg(0x0313, 0x02); CHECKERR;
    // set WARM threshold to 45C
    err = PMIC_write_reg(0x0314, 0x54); CHECKERR;
    err = PMIC_write_reg(0x0315, 0x01); CHECKERR;
    // set HOT threshold to 45C
    err = PMIC_write_reg(0x0316, 0x54); CHECKERR;
    err = PMIC_write_reg(0x0317, 0x01); CHECKERR;

    // set charging current to 800mA
    err = PMIC_write_reg(0x0308, 0xc8); CHECKERR;
    err = PMIC_write_reg(0x0309, 0x00); CHECKERR;
    // set charging termination voltage 4.2V
    err = PMIC_write_reg(0x030C, 0x08); CHECKERR;
    // enable charger
    err = PMIC_write_reg(0x0304, 0x03); CHECKERR;

    err = PMIC_read_reg(0x0410, &reg); CHECKERR;

    err = PMIC_read_reg(0x0411, &reg); CHECKERR;

    return err;
}

int npm1300_init(void)
{
    int err;

    err = power_mgmt_init();
    if (err) {
        return err;
    }

    return 0;
}
