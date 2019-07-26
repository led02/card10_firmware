/*MEMORY {
    SPIX (rx)  : ORIGIN = 0x08000000, LENGTH = 128M
    FLASH (rx) : ORIGIN = 0x10080000, LENGTH = 512k
    SRAM (rwx) : ORIGIN = 0x20040000, LENGTH = 256k
    SPID (r)   : ORIGIN = 0x80000000, LENGTH = 512M
}*/

ENTRY(isr_vector);

CARD10_CORE1_START = 0x20040000;
CARD10_CORE1_LIMIT = 0x20080000;

PHDRS
{
    header PT_PHDR PHDRS ;
    interp PT_INTERP ;
    text PT_LOAD FILEHDR PHDRS ;
    data PT_LOAD ;
    dynamic PT_DYNAMIC ;
}

SECTIONS {
    /* . = CARD10_CORE1_START; */

    . = SIZEOF_HEADERS;

    .cinterp :
    {
        *(.cinterp);
    } :interp :text

    .text :
    {
        *(.text*)
        *(.rodata*)
        
        KEEP(*(.init))
        KEEP(*(.fini))
    } :text

    .data :
    {
        . = ALIGN(4);
        *(.data*)

        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } :data

    .dynamic :
    {
        *(.dynamic)
    } :data :dynamic

    .bss :
    {
        . = ALIGN(4);
        CARD10_BSS_OFF_START = .;
        *(.bss*)
        *(COMMON)
        CARD10_BSS_LIMIT = .;
    } :data

    . = ALIGN(4096);
    .stack : 
    {
        CARD10_STACK_START = .;
        . += 4096;
        CARD10_STACK_LIMIT = .;
    } :data

    /DISCARD/ :
    {
        /* *(*) */
        /* *(.symtab) */
        /* *(.strtab) */
        /* *(.shstrtab) */
        /**(.interp)*/
        *(.comment)
        *(.interp)
    }
}
