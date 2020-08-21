SECTION .smp_btstrp
[BITS 64]
global smp_start
global smp_end

smp_start:
INCBIN "src/smp/smp_bootstrap.bin"
smp_end: