BITS 64

section .text
global _lgdt
global _lidt

_lgdt:
    lgdt [rdi]
    ; Reload CS register containing code selector:
    mov   rax, rsp
    push  ss
    push  rax
    pushfq
    push  0x08
    push _reload_cs
    iretq
_reload_cs:
    ; Reload data segment registers:
    mov   ax, 0x10 ; 0x10 points at the new data selector
    mov   ds, ax
    mov   es, ax
    mov   fs, ax
    mov   gs, ax
    mov   ss, ax
    ret

_lidt:
    lidt [rdi]
    ret