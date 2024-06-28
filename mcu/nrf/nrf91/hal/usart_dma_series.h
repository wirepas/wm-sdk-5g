/* Copyright 2024 Wirepas Ltd. All Rights Reserved.
 *
 * See file LICENSE.txt for full license details.
 *
 */


/**
 * \brief   Configure USART timers for nRF91 devices.
 */
__attribute__((__always_inline__)) static inline void configure_timers(void)
{
    /* Configure PPI: 3 channels used, configured in a group */

    /* Create group */
    NRF_DPPIC->CHG[0] = (DPPIC_CHG_CH3_Included << DPPIC_CHG_CH3_Pos)
                        | (DPPIC_CHG_CH4_Included << DPPIC_CHG_CH4_Pos)
                        | (DPPIC_CHG_CH5_Included << DPPIC_CHG_CH5_Pos);

    /* Start Timer 1 when RX is started. Only used one time when starting RX */
    NRF_UARTE0->PUBLISH_RXSTARTED = 3 << UARTE_PUBLISH_RXSTARTED_CHIDX_Pos
                                    | UARTE_PUBLISH_RXSTARTED_EN_Msk;

    NRF_TIMER1->SUBSCRIBE_START
        = 3 << TIMER_SUBSCRIBE_START_CHIDX_Pos | TIMER_SUBSCRIBE_START_EN_Msk;

    /* Reset timer 1, each time a byte is received to avoid Timeout */
    /* Count the number of bytes received with Timer2 in count mode */
    NRF_UARTE0->PUBLISH_RXDRDY
        = 4 << UARTE_PUBLISH_RXDRDY_CHIDX_Pos | UARTE_PUBLISH_RXDRDY_EN_Msk;

    NRF_TIMER1->SUBSCRIBE_CLEAR
        = 4 << TIMER_SUBSCRIBE_CLEAR_CHIDX_Pos | TIMER_SUBSCRIBE_CLEAR_EN_Msk;

    NRF_TIMER2->SUBSCRIBE_COUNT
        = 4 << TIMER_SUBSCRIBE_COUNT_CHIDX_Pos | TIMER_SUBSCRIBE_COUNT_EN_Msk;


    /* Clear the Timer2 when ENDRX happens, ie buffer wrap*/
    NRF_UARTE0->PUBLISH_ENDRX
        = 5 << UARTE_PUBLISH_ENDRX_CHIDX_Pos | UARTE_PUBLISH_ENDRX_EN_Msk;

    NRF_TIMER2->SUBSCRIBE_CLEAR
        = 5 << TIMER_SUBSCRIBE_CLEAR_CHIDX_Pos | TIMER_SUBSCRIBE_CLEAR_EN_Msk;
}
