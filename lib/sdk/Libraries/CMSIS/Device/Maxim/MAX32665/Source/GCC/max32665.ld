/*******************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Date: 2018-12-03 22:29:43 +0000 (Mon, 03 Dec 2018) $ 
 * $Revision: 39576 $
 *
 ******************************************************************************/


/* Allocate space if SDMA_SIZE is defined at the start of SRAM*/
SRAM_ORIGIN = 0x20000000;
SDMA_LEN = DEFINED(SDMA_SIZE) ? SDMA_SIZE : 0;
OTP_LEN = DEFINED(SDMA_SIZE) ? 0x40 : 0;
SHARED_LEN = DEFINED(SDMA_SIZE) ? 1k : 0;
SHARED_LEN_HALF = SHARED_LEN / 2;

OTP_ADDR = SRAM_ORIGIN;
SHARED_ADDR = OTP_ADDR + OTP_LEN;
SDMA_ADDR = SHARED_ADDR + SHARED_LEN;
SRAM_ADDR = SDMA_ADDR + SDMA_LEN;

/* Give leftover SRAM to the SRAM memory region */
/* SRAM_SIZE must be defined */
SRAM_LEN = SRAM_SIZE - OTP_LEN - SHARED_LEN - SDMA_LEN;

/* SPID and SPIX Sections here are maximum possible sizes */
/* If used, they should be adjusted for the external Flash/RAM size */
MEMORY {
    SPIX (rx)  : ORIGIN = 0x08000000,                           LENGTH = 128M
    FLASH (rx) : ORIGIN = 0x10010000,                           LENGTH = 960k

    OTP (rwx)       : ORIGIN = OTP_ADDR,                        LENGTH = OTP_LEN
    ARM_SHARED(rwx) : ORIGIN = SHARED_ADDR,                     LENGTH = SHARED_LEN_HALF
    SDMA_SHARED(rwx): ORIGIN = SHARED_ADDR + SHARED_LEN_HALF,   LENGTH = SHARED_LEN_HALF
    SDMA (rwx)      : ORIGIN = SDMA_ADDR,                       LENGTH = SDMA_LEN
    
    SRAM (rwx) : ORIGIN = SRAM_ADDR,                            LENGTH = SRAM_LEN
    SPID (rw)  : ORIGIN = 0x80000000,                           LENGTH = 512M
}

SECTIONS {
    .text :
    {
        _text = .;
        KEEP(*(.isr_vector))
        *(.text*)    /* program code */
        *(.rodata*)  /* read-only data: "const" */
        
        KEEP(*(.init))
        KEEP(*(.fini))

        /* C++ Exception handling */
        KEEP(*(.eh_frame*))
        _etext = .;
    } > FLASH
    
    /* This section will keep the SPIX data until loaded into the external device */
    /* Upon initialization of SPIX (user code needs to do this) */
    .xip_section :
    {
        KEEP(*(.xip_section*))
    } > SPIX AT>FLASH

    __load_start_xip = LOADADDR(.xip_section);
    __load_length_xip = SIZEOF(.xip_section);

    /* it's used for C++ exception handling      */
    /* we need to keep this to avoid overlapping */
    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } > FLASH

    .data :
    {
        _data = ALIGN(., 4);
        *(.data*)           /*read-write initialized data: initialized global variable*/
        *(.spix_config*)    /* SPIX configuration functions need to be run from SRAM */
        *(.flashprog*)      /* Flash program */
        

        /* These array sections are used by __libc_init_array to call static C++ constructors */
        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        _edata = ALIGN(., 4);
    } > SRAM AT>FLASH
    __load_data = LOADADDR(.data);

    .otp (NOLOAD) :
    {
        _otp = ALIGN(., 4);
        *(.otp*)           /*Copy of OTP memory*/

        _eotp = ALIGN(., 4);
    } > OTP

    .arm_shared (NOLOAD) :
    {
        _arm_shared = ALIGN(., 4);
        *(.arm_shared*)           /*Shared memory section*/

    _earm_shared = ALIGN(., 4);
    } > ARM_SHARED

    .sdma_shared (NOLOAD) :
    {
        _sdma_shared = ALIGN(., 4);
        *(.sdma_shared*)           /*Shared memory section*/

        _esdma_shared = ALIGN(., 4);
    } > SDMA_SHARED

    .sdma :
    {
        _sdma = ALIGN(., 4);
        *(.sdma_code*)           /*SDMA Code*/

        _esdma = ALIGN(., 4);
    } > SDMA AT>FLASH
    __load_sdma = LOADADDR(.sdma);

    .bss :
    {
        . = ALIGN(4);
        _bss = .;
        *(.bss*)     /*read-write zero initialized data: uninitialzed global variable*/
        *(COMMON)
        _ebss = ALIGN(., 4);
    } > SRAM

    /* Setup the stack for Core 1, it will only be used if the user code
     * includes a definition of Stack_Size_Core1, which defines the space
     * reserved above the main core's stack for core 1's stack */

    __StackTop_Core1 = ORIGIN(SRAM) + LENGTH(SRAM);
    __StackLimit_Core1 = DEFINED(Stack_Size_Core1) ? __StackTop_Core1 - Stack_Size_Core1 : __StackTop_Core1;

    /* Set stack top to end of RAM, and stack limit move down by Stack_Size. 
     * If core 1 is used, set the stack to the bottom of Core 1's stack region */

    __StackTop = DEFINED(Stack_Size_Core1) ? __StackLimit_Core1 : ORIGIN(SRAM) + LENGTH(SRAM);
    __StackLimit = __StackTop - Stack_Size;

    .heap (COPY):
    {
        . = ALIGN(4);
        *(.heap*)
        __HeapLimit = ABSOLUTE(__StackLimit);
    } > SRAM

    PROVIDE(__stack = __StackTop);

    /* Check if data + heap + stack(s) exceeds RAM limit */
    ASSERT(__StackLimit >= _ebss, "region RAM overflowed with stack")
}
