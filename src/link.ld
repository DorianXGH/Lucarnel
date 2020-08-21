OUTPUT_FORMAT("elf64-x86-64")
ENTRY(_entrypoint)
PHDRS
{
  main PT_LOAD AT ( 0x00100000 );
}
SECTIONS
{
  . = 0x00100000;
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

  .smp_btstrp : ALIGN(0x1000) {
_SMP_START_ = .;
    *(.smp_btstrp)
_SMP_END_ = .;
  } :main
  . = 0x001F0000;
  .long_btstrp :  {
    *(long_btstrp)
  } :main
}