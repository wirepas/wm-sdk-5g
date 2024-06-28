/* Copyright 2024 Wirepas Ltd. All Rights Reserved.
 *
 * See file LICENSE.txt for full license details.
 *
 */

/**
 * \file wms_testing.h
 * 
 * The Testing library provides a set of functions specific for testing purposes
*/
#ifndef APP_LIB_TESTING_H_
#define APP_LIB_TESTING_H_

#include <stdlib.h>
#include <stdint.h>

#include "wms_app.h"

/** \brief Library symbolic name */
#define APP_LIB_TESTING_NAME 0x56dc011c  //!< "TESTING"

/** \brief Maximum supported library version */
#define APP_LIB_TESTING_VERSION 0x200

/**
 * @brief   Disable downlink keepalive
 *
 * @note    This function is used by 5G Mesh Harmonized Standard Companion
 *          Device application
 */
typedef void (*app_lib_testing_disable_downlink_keepalive_f)(void);

/**
 * The function table returned from @ref app_open_library_f
 */
typedef struct
{
    /**
     * @brief   Function callback to disable downlink keepalive
     *
     * @note    TESTING ONLY - to be used by 5G Mesh Harmonized Standard
     *          Companion Device application
     */
    app_lib_testing_disable_downlink_keepalive_f   disableDownlinkKeepalive;
} app_lib_testing_t;

#endif /* APP_LIB_TESTING_H_ */
