GCC_ASM_ARGS:= -masm=intel
GAS_ASM_ARGS:= -msyntax=intel -mnaked-reg
GCC_TARGET_ARGS:= -ffreestanding -mno-red-zone -std=c11
GGC_INTERRUPT_ARGS:= -mgeneral-regs-only
GCC_TOOLS_PREFIX:= /home/dorian/opt/cross/bin/x86_64-elf-


build/main.o: src/main.c
	$(GCC_TOOLS_PREFIX)gcc -o $@ -c $< $(GCC_TARGET_ARGS) $(GCC_ASM_ARGS)

build/video.o: src/video/video.c
	$(GCC_TOOLS_PREFIX)gcc -o $@ -c $< $(GCC_TARGET_ARGS) $(GCC_ASM_ARGS)

build/paging.o: src/memory/paging.c
	$(GCC_TOOLS_PREFIX)gcc -o $@ -c $< $(GCC_TARGET_ARGS) $(GCC_ASM_ARGS)

build/ISR.o: src/interrupts/ISR.c
	$(GCC_TOOLS_PREFIX)gcc -o $@ -c $< $(GCC_TARGET_ARGS) $(GCC_ASM_ARGS) $(GGC_INTERRUPT_ARGS)

build/entry.o: src/entry.S
	nasm $< -o $@ -f elf64

build/x86_64_utils.o: src/x86_64_utils.S
	nasm $< -o $@ -f elf64

build/kernel.elf: build/main.o build/entry.o build/video.o build/x86_64_utils.o build/paging.o build/ISR.o
	$(GCC_TOOLS_PREFIX)ld -m elf_x86_64 -T src/link.ld $^ -o $@

clean:
	rm -rf build/*