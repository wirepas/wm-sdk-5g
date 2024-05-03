/*
 * \file    app.c
 * \brief   This file is EUT-PT device application for
 *          5G Mesh harmonized standard channel accsess testing
 * 
 * 1. Start the stack
 * 2. Periodic UL packet sending
 * 3. LED#4 is set ON when UL packet is sent
 * 4. LED#4 is set OFF when there is no route to sink and stop
 * sending UL packets
 */

#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "shared_data.h"
#include "led.h"
#include "app_scheduler.h"
#include "stack_state.h"
#include "node_configuration.h"

// LED #4
#define LED_ID      3

// EPs
// These EP should be the same with those from EUT Companion device app
#define SRC_ENDPOINT    11
#define DST_ENDPOINT    11

/* UL packet sending period */
uint32_t m_interval = PERIODIC_TASK_INTERVAL_MS;

/**
 * \brief   This callback is to be informed when UL packet is sent
*/
static void data_sent_cb(const app_lib_data_sent_status_t * status)
{
    /* If packet is successfully sent, turn on the LED#4 */
    if (status->success)
    {
        Led_set(LED_ID, true);
    }
}

/**
 * \brief   This is for periodic UL packet sending
*/
static uint32_t periodic_send_ul_packet(void)
{
    app_lib_state_route_info_t route_info;

    /* Only send data if there is a route to the Sink. */
    app_res_e res = lib_state->getRouteInfo(&route_info);
    if (res == APP_RES_OK && route_info.state == APP_LIB_STATE_ROUTE_STATE_VALID)
    {
        uint8_t buff = 0xFF;
        app_lib_data_to_send_t data_to_send;
        data_to_send.bytes = &buff;
        data_to_send.num_bytes = 1;
        data_to_send.dest_address = APP_ADDR_ANYSINK;
        data_to_send.src_endpoint = SRC_ENDPOINT;
        data_to_send.dest_endpoint = DST_ENDPOINT;
        data_to_send.qos = APP_LIB_DATA_QOS_HIGH;
        data_to_send.flags = APP_LIB_DATA_SEND_FLAG_NONE;
        data_to_send.tracking_id = APP_LIB_DATA_NO_TRACKING_ID;

        /* Send the data packet. */
        Shared_Data_sendData(&data_to_send, data_sent_cb);
    }
    else
    {
        /* If there is no route to sink or cannot get route info, turn off LED#4 */
        Led_set(LED_ID, false);
    }
    return m_interval;
}

/**
 * \brief   This callback is to be informed when route changed
 *          Then it will check if the route to sink is still valid
*/
static void route_changed_cb(app_lib_stack_event_e event, void * param)
{
    app_lib_state_route_info_t * route_info = (app_lib_state_route_info_t *) param;

    /* If there is no route to sink, turn off the LED#4 and stop sending UL */
    if (route_info->state == APP_LIB_STATE_ROUTE_STATE_INVALID)
    {
        Led_set(LED_ID, false);
        App_Scheduler_cancelTask(periodic_send_ul_packet);
    }
    else if (route_info->state == APP_LIB_STATE_ROUTE_STATE_VALID)
    {
        App_Scheduler_addTask_execTime(periodic_send_ul_packet, APP_SCHEDULER_SCHEDULE_ASAP, 500);
    }
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
    // PT device in LL Non-Router
    lib_settings->setNodeRole(APP_LIB_SETTINGS_ROLE_SUBNODE_LL);

    // Basic configuration of the node with a unique node address
    if (configureNodeFromBuildParameters() != APP_RES_OK)
    {
        return;
    }

    // Route changed callback
    Stack_State_addEventCb(route_changed_cb, 1 << APP_LIB_STATE_STACK_EVENT_ROUTE_CHANGED);

    // Start the stack
    lib_state->startStack();

    // Periodic UL packet sending
    App_Scheduler_addTask_execTime(periodic_send_ul_packet, APP_SCHEDULER_SCHEDULE_ASAP, 500);
}
