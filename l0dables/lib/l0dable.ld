ENTRY(__isr_vector);

/*
 * Segment in the output l0dable.
 *
 * They are mostly standard, but we define them explicitely so that we can
 * target them in sections.
 */
PHDRS
{
    header PT_PHDR PHDRS ;
    interp PT_INTERP ;
    text PT_LOAD FILEHDR PHDRS ;
    data PT_LOAD ;
}

/*
 * ELF sections.
 */
SECTIONS {
    . = SIZEOF_HEADERS;

    /*
     * Customer card10-l0dable INTERP/intepreter path.
     *
     * We nuke the original one (.interp) provided by gcc/ld, and inject out
     * own. This section is populated in l0dable.ld.
     */
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
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);


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

    .bss :
    {
        . = ALIGN(4);
        *(.bss*)
        *(COMMON)
    } :data

    . = ALIGN(4096);
    .stack : 
    {
        CARD10_STACK_START = .;
        . += 8192;
        CARD10_STACK_LIMIT = .;
    } :data

    /* Limit based on current limitations of l0dable setup - only uses core1 RAM. */
    ASSERT(. < 0x40000, "Exceeded available RAM")

    /DISCARD/ :
    {
        /* Compiler version - nuke. */
        *(.comment)
        /* ARM attributes - nuke. */
        *(.ARM.attributes)
        /* Original interpreter path from gcc/ld - nuke. */
        *(.interp)
        /* Dynamic linking section - nuke, we're not a .so and nothing is going to link against us. */
        *(.dynamic)
    }
}
