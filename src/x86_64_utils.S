BITS 64

section .text
global _lgdt
global _lidt

_lgdt:
    lgdt [rdi]
    ret

_lidt:
    lidt [rdi]
    ret