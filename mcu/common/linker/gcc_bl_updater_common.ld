/* Copyright 2024 Wirepas Ltd. All Rights Reserved.
 *
 * See file LICENSE.txt for full license details.
 *
 */

/* Linker script for Wirepas Generic Bootloader Updater Tool, common parts
 */

SECTIONS
{
    .text :
    {
        __rom_start__ = .;
        KEEP(*(.entrypoint))
        *(.romtext)
        *(.text.*)
        *(.text)
        *(.glue_7)
        *(.glue_7t)
        *(.vfp11_veneer)
        *(.v4_bx)
        *(.rodata.*)
        *(.rodata)
        *(.rodata1)
        *(.crc_lut)
        . = ALIGN(8);
    } >FLASH

    __etext = .;
    __data_src_start__ = .;

    .data :
    {
        __data_start__ = .;
        *(.ramtext)
        *(.data.*)
        *(.data)
        *(.data1)
        . = ALIGN(8);
        __data_end__ = .;
    } >RAM AT >FLASH

    __rom_end__ = __data_src_start__ + SIZEOF(.data);

    .config :
    {
        /* Generic Bootloader Updater Tool configuration */
        __config_start__ = .;
        . += LENGTH(CONFIG);
        __config_end__ = .;
    } >CONFIG

    .bss :
    {
        __bss_start__ = .;
        *(.bss.*)
        *(.bss)
        *(COMMON)
        . = ALIGN(8);
        __bss_end__ = .;
    } >RAM

    .breason_sguard (NOLOAD) :
    {
        *(.breason_sguard)
    } >BREASON_SGUARD

    /* .stack_dummy section doesn't contain any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later */
    .stack_dummy (COPY):
    {
        *(.stack*)
    } > STACK

    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __stack_top__ = ORIGIN(STACK) + LENGTH(STACK);

    .invalid :
    {
        *(.init)
        *(.fini)
        *(.preinit_array)
        *(.init_array)
        *(.fini_array)
        *(.ctors)
        *(.dtors)
        *(.jcr)
        *(.eh_frame)
        *(.eh_frame_hdr)
        *(.heap*)
        *(.tbss)
        *(.tdata)
        *(.tdata1)
        *(.got)
        *(.got.plt)
        *(.igot.plt)
        *(.iplt)
        *(.rel.dyn)
        *(.rel.iplt)
        *\libgcc.a:*df*.o
        *\libgcc.a:*sf*.o
    } >INVALID  /* Linking fails if any of these sections have contents */

    /* C++ exception unwinding information is silently discarded */
    /DISCARD/ :
    {
        *(.ARM.extab)
        *(.ARM.extab.*)
        *(.ARM.exidx)
        *(.ARM.exidx.*)
    }
}
