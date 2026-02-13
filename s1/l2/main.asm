section .text
global _start

_start:
    mov rdi, 1
    mov rsi, text
    mov rdx, text_len
    mov rax, 1
    syscall


    mov rax, 60
    mov rdi, 0
    syscall

section .data
text:
    db "Hello, world", 10
text_len equ $-text
