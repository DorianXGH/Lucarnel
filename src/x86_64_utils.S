BITS 64

section .text
global _lgdt
global _lidt
global _lcr3

_lgdt:
    lgdt [rdi]
    ; Reload CS register containing code selector:
    mov rax, rsp
	push qword 0x10
	push qword rax
	pushf
	push qword 0x08
	push qword _reload_cs
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

_lcr3:
    mov cr3, [rdi]
    ret