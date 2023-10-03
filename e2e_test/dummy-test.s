.option norvc
.section .text
.global start
start:
    addi a0, zero, 1
    jalr zero, ra, 0
