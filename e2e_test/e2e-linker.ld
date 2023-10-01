OUTPUT_ARCH("riscv")
ENTRY(_start)

SECTIONS {
    . = 0x000;
    .text.init : { *(.text.init) }
    .text : { *(.text) }
    .data : ALIGN(0x1000) { *(.data .rodata*) }
    .bss : ALIGN(0x1000) { *(.bss*) *(COMMON) }

    _end = .;
}
