; boot.s - Multiboot-compliant 32-bit x86 entry for GRUB

BITS 32

section .multiboot
align 4
    dd 0x1BADB002               ; Multiboot magic
    dd 0x00000003               ; Flags: align modules + memory info
    dd -(0x1BADB002 + 0x00000003) ; Checksum

section .text
global _start
extern kmain

_start:
    cli
    mov esp, stack_top
    call kmain

.hang:
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384                  ; 16 KiB kernel stack
stack_top:
