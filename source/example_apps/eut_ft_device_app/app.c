/*
 * \file    app.c
 * \brief   This file is EUT-FT device application for
 *          5G Mesh harmonized standard channel accsess testing
 * 
 * 1. Start the stack
 * 2. LED#1 is set ON when stack is started
 */
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "led.h"
#include "stack_state.h"
#include "node_configuration.h"

#define LED_ID      0

/**
 * \brief   This callback is to be informed when stack is stopped
*/
static void stack_stopped_cb(app_lib_stack_event_e event, void * param)
{
    // If stack stops, turn of LED#1
    Led_set(LED_ID, false);
}

/**
 * \brief   Initialization callback for application
 *
 * This function is called after hardware has been initialized but the
 * stack is not yet running.
 *
 */
void App_init(const app_global_functions_t * functions)
{
    // FT mode = LL sink
    lib_settings->setNodeRole(APP_LIB_SETTINGS_ROLE_SINK_LL);

    // Basic configuration of the node with a unique node address
    if (configureNodeFromBuildParameters() != APP_RES_OK)
    {
        return;
    }

    // Set callback when stack is stopped
    Stack_State_addEventCb(stack_stopped_cb, 1 << APP_LIB_STATE_STACK_EVENT_STACK_STOPPED);

    // Start the stack
    app_res_e res = lib_state->startStack();

    // If stack starts successfully, turn on the LED#1
    if (res == APP_RES_OK)
    {
        Led_set(LED_ID, true);
    }
}
