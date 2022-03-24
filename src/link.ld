OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_entrypoint)
PHDRS
{
  main PT_LOAD;
}
SECTIONS
{
  . = 0xffffffff80200000;
  .stivalehdr : {
    *(.stivalehdr)
  } :main

  .text.start : {
    *(.text.start)
  } :main

  .text : ALIGN(0x1000) {
_TEXT_START_ = .;
    *(.text)
_TEXT_END_ = .;
  } :main

  .data : ALIGN(0x1000) {
_DATA_START_ = .;
    *(.data)
_DATA_END_ = .;
  } :main

  .bss : ALIGN(0x1000) {
_BSS_START_ = .;
    *(.bss)
_BSS_END_ = .;
  } :main

  .stack : ALIGN(0x1000) {
_STACK_START_ = .;
    *(.stack)
_STACK_END_ = .;
  } :main
}