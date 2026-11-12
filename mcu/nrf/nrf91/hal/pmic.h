#ifndef _PMIC_H_
#define _PMIC_H_

#include<stdint.h>
#include<stdbool.h>

typedef enum 
{
    PMIC_RES_OK,                  ///< Last operation was successful
    PMIC_RES_ERROR,               ///< Operation failed
} pmic_res_e;

/**
 * Set configuration for using the PMIC
 * \param i2c_clock_freq    Clock frequency [Hz] of the I2C bus serial clock line (SCL)
 * \param i2c_pullup        Set to true to enable GPIO internal pull up resistors for SCL and SDA
 * \param i2c_address       I2C address of the pmic device (used in every read and write)
 * \return PMIC_RES_OK
 */
pmic_res_e PMIC_configure(uint32_t i2c_clock_freq, bool i2c_pullup, uint8_t i2c_address);

/**
 * Set configuration for using the PMIC
 * \param pmic_address    Register address on the PMIC device where to write
 * \param value           Value to be written
 * \return PMIC_RES_OK    Succesful write operation
 * \return PMIC_RES_ERROR Write operation failed
 */
pmic_res_e PMIC_write_reg(uint16_t pmic_address, uint8_t value);

/**
 * Set configuration for using the PMIC
 * \param pmic_address    Register address on the PMIC device from where to read
 * \param value           Pointer to address where value is written
 * \return PMIC_RES_OK    Succesful read operation
 * \return PMIC_RES_ERROR Read operation failed
 */
pmic_res_e PMIC_read_reg(uint16_t pmic_address, uint8_t *value);

#endif // _PMIC_H_
