/*
 * \file    app.c
 * \brief   This file is EUT companion device application for
 *          5G Mesh harmonized standard channel accsess testing
 * 
 * 1. Start the stack
 * 2. Constant query of cluster channel
 * 3. Button #1 press triggers that the current cluster channel in use is set as reserved
 * 4. LED#1:
 *  a. Cluster channel after boot is marked as channel A
 *  b. LED#1 is set ON when cluster channel A is in use
 *  c. LED#1 is set OFF when cluster channel A is not anymore in use
 * 5. LED#2:
 *  a. When cluster channel is changed from channel A (to another cluster channel named cluster channel B)
 *  this LED#2 is indicating this change
 *  b. LED#2 is set ON when cluster channel B is in use
 *  c. LED#2 is set OFF when cluster channel B is not anymore in use
 * 6. LED#3 on when receiving UL packets
 */

#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "shared_data.h"
#include "led.h"
#include "button.h"
#include "app_scheduler.h"
#include "stack_state.h"
#include "node_configuration.h"

#define DEBUG_LOG_MODULE_NAME "COMPANION_DEV_APP"
#define DEBUG_LOG_MAX_LEVEL LVL_INFO
#include "debug_log.h"

// LED #1 for indicating that channel A is in use
#define LED_CHANNEL_A   0
// LED #2 for indicating that channel B is in use
#define LED_CHANNEL_B   1
// LED #3 for indicating recieved UL packets
#define LED_UL_RX       2

// EPs
// These EP should be the same with those from EUT PT device app
#define SRC_ENDPOINT    11
#define DST_ENDPOINT    11

// Function foward declaration
// Packet received callback function
static app_lib_data_receive_res_e packet_received_cb(const shared_data_item_t * item,
                                                    const app_lib_data_received_t * data);

// Cluster channel in used when stack starts (named as channel A)
app_lib_settings_net_channel_t m_cluster_channel_a;

// Cluster channel after 1st time pressing button (named as channel B)
app_lib_settings_net_channel_t m_cluster_channel_b;

// Channel array for marking reserved channels
// Max channel count in DECT is 11, thus need 2 bytes for channel bit array
uint8_t m_resv_chns[2] = {0};

// Interval of querying cluster channel (in ms)
uint32_t m_interval_ms = 1000;

// First query of cluster channel (after boot)
bool m_query_cluster_channel_after_boot = false;

// Channel B has been set or not
bool m_is_channel_b_set = false;

// Button pressed
bool m_button_pressed = false;

// Received packets filter and callback
shared_data_item_t pkt_filter =
{
    .cb = packet_received_cb,
    .filter = {
                .mode = SHARED_DATA_NET_MODE_ALL,
                .src_endpoint = SRC_ENDPOINT,
                .dest_endpoint = DST_ENDPOINT,
                .multicast_cb = NULL
              }
};

/**
 * \brief   Periodic querying of cluster channel currently in use
*/
static uint32_t periodic_query_cluster_channel(void)
{
    // Query cluster channel currently in use
    app_lib_settings_net_channel_t cluster_channel = lib_state->getClusterChannel();
    
    // Store the channel after boot as channel A
    if (m_query_cluster_channel_after_boot)
    {
        m_cluster_channel_a = cluster_channel;
        m_query_cluster_channel_after_boot = false;
    }

    // Store channel after button pressed first time as channel B
    if (m_button_pressed &&
        (!m_is_channel_b_set) &&
        (cluster_channel != m_cluster_channel_a))
    {
        m_cluster_channel_b = cluster_channel;
        m_button_pressed = false;
        m_is_channel_b_set = true;
    }
    LOG(LVL_INFO, "Cluster channel query: %u", cluster_channel);

    if (cluster_channel == m_cluster_channel_a)
    {
        // Set LED #1 on, turn off LED #2
        Led_set(LED_CHANNEL_A, true);
        Led_set(LED_CHANNEL_B, false);
    }
    else if (cluster_channel == m_cluster_channel_b)
    {
        // Set LED #2 on, turn off LED #1
        Led_set(LED_CHANNEL_B, true);
        Led_set(LED_CHANNEL_A, false);
    }
    else
    {
        Led_set(LED_CHANNEL_A, false);
        Led_set(LED_CHANNEL_B, false);
    }

    return m_interval_ms;
}

/**
 * \brief   This function is triggered when button #1 is pressed
 *          to set the current cluster channel in use as reserved
*/
static void button_pressed_handler(uint8_t button_id, button_event_e event)
{
    // Get cluster channel currently in use
    app_lib_settings_net_channel_t cluster_channel = lib_state->getClusterChannel();
    // Mark current cluster channel as reserved
    uint8_t byte_index = (cluster_channel - 1) / 8;
    uint8_t bit_offset = (cluster_channel - 1) % 8;
    m_resv_chns[byte_index] |= (1 << bit_offset);
    lib_settings->setReservedChannels(m_resv_chns, sizeof(m_resv_chns));

    m_button_pressed = true;
}

/**
 * \brief   Function to turn off LED #3 after receiving packet
*/
static uint32_t led_ul_rx_off_cb(void)
{
    Led_set(LED_UL_RX, false);
    return APP_SCHEDULER_STOP_TASK;
}

/**
 * \brief   Callback function when a packet is received
*/
static app_lib_data_receive_res_e packet_received_cb(const shared_data_item_t * item,
                                                    const app_lib_data_received_t * data)
{
    LOG(LVL_INFO, "UL Pkt rx")
    // Set LED #3 on when receiving pkts
    Led_set(LED_UL_RX, true);
    App_Scheduler_addTask_execTime(led_ul_rx_off_cb, 200, 500);

    // Data handled successfully
    return APP_LIB_DATA_RECEIVE_RES_HANDLED;
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
    LOG_INIT();

    // Companion device is LL Sink
    lib_settings->setNodeRole(APP_LIB_SETTINGS_ROLE_SINK_LL);

    // Basic configuration of the node with a unique node address
    if (configureNodeFromBuildParameters() != APP_RES_OK)
    {
        return;
    }

    // Disable downlink keep-alive message
    lib_testing->disableDownlinkKeepalive();
    
    // Register button pressed event on button #1
    Button_register_for_event(0,
                            BUTTON_PRESSED,
                            button_pressed_handler);

    // Register cb for rx packets
    Shared_Data_addDataReceivedCb(&pkt_filter);

    // Start the stack
    lib_state->startStack();

    // Cluster channel after boot
    m_query_cluster_channel_after_boot = true;
    // Periodic query of cluster channel
    // Delay the start of cluster channel querying by few seconds to
    // avoid the cluster channel queried is 0
    App_Scheduler_addTask_execTime(periodic_query_cluster_channel,
                                    3000,
                                    500);
}
