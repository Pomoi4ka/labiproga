.global _start
.text

.equ SYS_EXIT, 93
.equ SYS_WRITE, 64
.equ PRINT_NUMBER_BASE, 10
.equ NEWLINE, 10
.equ ASCII_ZERO, 48

j _start
strlen:
    mv t0, a0
strlen_again:
    lb t1, 0(a0)
    beq t1, x0, strlen_done
    addi a0, a0, 1
    j strlen_again
strlen_done:
    sub a0, a0, t0
    ret

print:
    addi sp, sp, -8
    sw ra, 0(sp)

    sw a0, 4(sp)
    call strlen
    mv a2, a0
    lw a1, 4(sp)
    li a0, 1
    li a7, SYS_WRITE
    ecall

    lw ra, 0(sp)
    addi sp, sp, 8
    ret

printNumber:
    addi sp, sp, -64
    sw ra, 0(sp)

    addi t1, sp, 60
    li t2, PRINT_NUMBER_BASE
    li t3, NEWLINE

    addi t1, t1, -2
    sh t3, 0(t1)

printNumber_loop:
    rem t3, a0, t2
    div a0, a0, t2

    addi t3, t3, ASCII_ZERO
    addi t1, t1, -1
    sb t3, 0(t1)

    bne a0, x0, printNumber_loop

    mv a0, t1
    call print

    lw ra, 0(sp)
    addi sp, sp, 64
    ret

procedure:
    srli a0, a0, 2
    addi a1, a1, -1
    slli a1, a1, 3
    add a0, a0, a1
    ret

_start:
    la s0, vars
    lw a0, X_VARS_OFFSET(s0)
    lw a1, Y_VARS_OFFSET(s0)

    call procedure
    call printNumber

    li a0, 0
    li a7, SYS_EXIT
    ecall

.data

vars:
    x: .word 69
    y: .word 42

.equ X_VARS_OFFSET, 0 # x - vars is not supported >:L
.equ Y_VARS_OFFSET, 4
