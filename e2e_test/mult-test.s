.option norvc
.section .text
.global start

// multiplies two unsigned dwords
// a1 - first multiplier
// a2 - second multplier
// a0 - result
mul:
    // leaf function, can reduce stack frame creation

    add a0, zero, zero

.mul_loop:
    andi t0, a2, 0x1
    beqz t0, .mul_next
    add a0, a0, a1
.mul_next:
    srli a2, a2, 0x1
    slli a1, a1, 0x1

    bnez a2, .mul_loop

.mul_end:
    jalr zero, ra, 0

start:
    // creating stack frame of 8 bytes size
    // to store ra register value onto the stack
    addi sp, sp, -8
    // store ra onto the stack
    sd ra, 0(sp)

    // load operands of mul function: want to get value of 15 * 8
    li a1, 15
    li a2, 8
    call mul
    
    li a1, 120
    sub a0, a0, a1
    bnez a0, .fail

.success:
    li a0, 1
    j .finish
    
.fail:
    li a0, 0

.finish:
    ld ra, 0(sp)
    add sp, sp, 8
    jalr zero, ra, 0
