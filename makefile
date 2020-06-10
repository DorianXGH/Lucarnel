GCC_ASM_ARGS:= -masm=intel
GAS_ASM_ARGS:= -msyntax=intel -mnaked-reg
GCC_TARGET_ARGS:= -ffreestanding -mno-red-zone 
GCC_TOOLS_PREFIX:= /home/dorian/opt/cross/bin/x86_64-elf-


build/main.o: src/main.c
	$(GCC_TOOLS_PREFIX)gcc -o $@ -c $< $(GCC_TARGET_ARGS) $(GCC_ASM_ARGS)

build/entry.o: src/entry.S
	nasm $< -o $@ -f elf64

build/kernel.elf: build/main.o build/entry.o
	$(GCC_TOOLS_PREFIX)ld -m elf_x86_64 -T src/link.ld $^ -o $@

clean:
	rm -rf build/*