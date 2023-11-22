.option norvc
.section ".text.init"
.global _start
.global start

// entry point
_start:
    // zero all general purpose registers
    add x1, x0, x0
    add x2, x0, x0
    add x3, x0, x0
    add x4, x0, x0
    add x5, x0, x0
    add x6, x0, x0
    add x7, x0, x0
    add x8, x0, x0
    add x9, x0, x0
    add x10, x0, x0
    add x11, x0, x0
    add x12, x0, x0
    add x13, x0, x0
    add x14, x0, x0
    add x15, x0, x0
    add x16, x0, x0
    add x17, x0, x0
    add x18, x0, x0
    add x19, x0, x0
    add x20, x0, x0
    add x21, x0, x0
    add x22, x0, x0
    add x23, x0, x0
    add x24, x0, x0
    add x25, x0, x0
    add x26, x0, x0
    add x27, x0, x0
    add x28, x0, x0
    add x29, x0, x0
    add x30, x0, x0
    add x31, x0, x0

    // load stack pointer into stack pointer register. The stack grows
    // down, so we need to place upper address there

    // lui sp, %hi(_bootstrap_stack_end)
    // addi sp, sp, %lo(_bootstrap_stack_end)
    la sp, _bootstrap_stack_end // THE POWER OF SPEUDO INSTRUCTIONS!
    
    // pass control to the test
    // auipc t0, %pcrel_hi(start)
    // addi t0, t0, %pcrel_lo(start)
    // jalr ra, t0
    call start // THE POWER OF PSEUDO INSTRUCTIONS!

    ebreak

.section .data

// allocate 4K aligned memory page for stack
_bootstrap_stack:
.skip 4096
_bootstrap_stack_end:

