/* Copyright 2024 Wirepas Ltd. All Rights Reserved.
 *
 * See file LICENSE.txt for full license details.
 *
 */

/** Define UART0_IRQn for the nRF91 series. */
#define UART0_IRQn UARTE0_SPIM0_SPIS0_TWIM0_TWIS0_IRQn

/* Define TASKS for the nRF91 series. */
#define NRF_UARTE0_TASKS_STARTTX (NRF_UARTE0->TASKS_STARTTX)
#define NRF_UARTE0_TASKS_STOPTX  (NRF_UARTE0->TASKS_STOPTX)
#define NRF_UARTE0_TASKS_STARTRX (NRF_UARTE0->TASKS_STARTRX)
#define NRF_UARTE0_TASKS_STOPRX  (NRF_UARTE0->TASKS_STOPRX)

/* Define EVENTS for the nRF91 series. */
#define NRF_UARTE0_EVENTS_ENDTX     (NRF_UARTE0->EVENTS_ENDTX)
#define NRF_UARTE0_EVENTS_ENDRX     (NRF_UARTE0->EVENTS_ENDRX)
#define NRF_UARTE0_EVENTS_RXSTARTED (NRF_UARTE0->EVENTS_RXSTARTED)

/* Define RX/TX buffer registers for the nRF91 series. */
#define NRF_UARTE0_RX_PTR    (NRF_UARTE0->RXD.PTR)
#define NRF_UARTE0_RX_MAXCNT (NRF_UARTE0->RXD.MAXCNT)
#define NRF_UARTE0_TX_PTR    (NRF_UARTE0->TXD.PTR)
#define NRF_UARTE0_TX_MAXCNT (NRF_UARTE0->TXD.MAXCNT)

/* Define interrupts for the nRF91 series. */
#define NRF_UARTE0_INTENSET                                                    \
    ((UARTE_INTEN_ENDTX_Enabled << UARTE_INTEN_ENDTX_Pos)                      \
     | (UARTE_INTEN_ERROR_Enabled << UARTE_INTEN_ERROR_Pos))

/* Define shortcuts for the nRF91 series. */
#define NRF_UARTE0_SHORTS                                                      \
    (UARTE_SHORTS_ENDRX_STARTRX_Enabled << UARTE_SHORTS_ENDRX_STARTRX_Pos)


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
