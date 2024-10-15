/* Copyright 2018 Wirepas Ltd. All Rights Reserved.
 *
 * See file LICENSE.txt for full license details.
 *
 */

#include <string.h>
#include "../bootloader_test/api/bl_interface.h"
#include "../bootloader_test/print/print.h"
#include "../bootloader_test/tests/test.h"
#include "../bootloader_test/timing/timing.h"

#include "em_chip.h"

#if(_SILICON_LABS_32B_SERIES_2_CONFIG == 1)
#include "interrupt.h"
#include "hal_interrupt.h"
#define SW_IRQn     SW0_IRQn
#endif

/** Addresses determined by the linker */
extern unsigned int __data_src_start__;
extern unsigned int __data_start__;
extern unsigned int __data_end__;
extern unsigned int __bss_start__;
extern unsigned int __bss_end__;

/* We need to reserve some space for the application header. It is not used but
 * otherwise genScratchpad.py would overwrite this area.
 */
const uint32_t info_hdr[8] __attribute__(( section (".app_header")));


#if(_SILICON_LABS_32B_SERIES_2_CONFIG == 1)
void Interrupt_init(void)
{
    IRQn_Type irq_number;

    /* Mask all maskable interrupt sources */
    for (irq_number = EMU_IRQn;
         irq_number <= MAX_IRQ_NUMBER;
         irq_number++)
    {
        NVIC_DisableIRQ(irq_number);
        NVIC_ClearPendingIRQ(irq_number);
    }

    /* Mask SW interrupt source */
    NVIC_DisableIRQ(SW_IRQn);
    NVIC_ClearPendingIRQ(SW_IRQn);
    NVIC_SetPriority(SW_IRQn, HAL_SWIRQ_INTERRUPT_PRIO);

    /* Enable global interrupts
     * Debug request resets only the core, not peripherals, so interrupts might
     * still be disabled */
    Mcu_globalInterruptEnable();
}
#endif
/**
 * \brief   The bootloader test application.
 */
void bootloader_test(bl_interface_t * interface)
{
    unsigned int * src, * dst;
    bool final_res = true;

    /* Copy data from flash to RAM */
    for(src = &__data_src_start__,
        dst = &__data_start__;
        dst != &__data_end__;)
    {
        *dst++ = *src++;
    }

    /* Initialize the .bss section */
    for(dst = &__bss_start__; dst != &__bss_end__;)
    {
        *dst++ = 0;
    }

    Timing_init();
    Print_init();

    Print_printf("\n\n #######################################\n");
    Print_printf(    " #                                     #\n");
    Print_printf(    " #      Starting bootloader tests      #\n");
    Print_printf(    " #                                     #\n");
    Print_printf(    " #######################################\n\n");

    Print_printf("Bootloader version is %d\n", interface->version);

    final_res &= Tests_info(interface);
    final_res &= Tests_areas(interface);
    final_res &= Tests_timings(interface);

    Print_printf("\n\n #######################################\n");
    Print_printf(    " #                                     #\n");
    Print_printf(    " #         Final result is %s        #\n",
                                                final_res ? "PASS" : "FAIL");
    Print_printf(    " #                                     #\n");
    Print_printf(    " #######################################\n\n");

    while(1);

}


/**
 * \brief   Entrypoint from bootloader
 */
void __attribute__ ((noreturn, section (".entrypoint")))
                                        entrypoint(bl_interface_t * interface)
{
#if(_SILICON_LABS_32B_SERIES_2_CONFIG == 1)
    Interrupt_init();
#endif   
    bootloader_test(interface);

    while(1);
}
