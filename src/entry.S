BITS 64
EXTERN main

section .text.start
global _entrypoint
_entrypoint :
    call main
    nop
    hlt