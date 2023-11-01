.option norvc
.section .text
.global start

.equ MSTATUS, 0x300
.equ MEPC, 0x341
.equ MSTATUS_MPP_MASK, (0b11 << 11)

.align 4
trap_vector:
    // we got into the trap, success!
    li a0, 1
    jalr zero, ra, 0

.align 4
start:
    // setting up trap vector address
    la t0, trap_vector
    csrw mtvec, t0
    // entering to the user mode
    la t0, usermode_entry
    csrw mepc, t0
    mret

    // dead code, that successfully terminates program with 0 error code
    li a0, 0
    jalr zero, ra, 0

.align 4
usermode_entry:
    // here we trying to write value to mstatus register
    // from user mode, that should lead to illegal instruction exception
    // raising

    // so, I excepect the programm falled with std::terminate now
    la t0, MSTATUS_MPP_MASK
    csrw mstatus, t0

    // dead code, that successfully terminates program with 0 error code
    li a0, 0
    jalr zero, ra, 0
