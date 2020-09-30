import os

GCC_ASM_ARGS = "-masm=intel"
GAS_ASM_ARGS = "-msyntax=intel -mnaked-reg"
GCC_TARGET_ARGS = "-ffreestanding -mno-red-zone -std=c11 -Werror -fcompare-debug-second"
GCC_INTERRUPT_ARGS = "-mgeneral-regs-only"
GCC_TOOLS_PREFIX = "/home/dorian/opt/cross/bin/x86_64-elf-"

c_sources = [
    "src/main.c", 
    "src/video/video.c", 
    "src/memory/paging.c", 
    "src/utils.c", 
    "src/smp/smp.c", 
    "src/interrupts/apic.c", 
    "src/acpi.c", 
    ]
interrupts_sources = [ 
    "src/interrupts/ISR.c", 
    "src/interrupts/IRQ.c", 
    "src/interrupts/IDT.c", 
    ]

asm_sources = [
    "src/entry.S", 
    "src/x86_64_utils.S"
    ]

def generate_deps(c_source):
    stream = os.popen(GCC_TOOLS_PREFIX + "gcc -MM " + c_source)
    output = stream.read()
    deps = output.split(' ')
    real_deps = [dep.strip("\n") for dep in deps if (not dep.endswith(".o:") and dep != "\\\n")]
    real_deps.append(c_source)
    return real_deps

def generate_obj_path(source):
    path = source.split('/')
    filename = path[-1]
    return "build/" + filename.rstrip("c").rstrip(".") + ".o"

def task_compile_c():
    for c_source in c_sources:
        yield {
            'name': c_source,
            'file_dep': generate_deps(c_source),
            'targets': [generate_obj_path(c_source)],
            'actions': ['%sgcc -o %s -c %s %s %s' % (GCC_TOOLS_PREFIX, generate_obj_path(c_source), c_source, GCC_TARGET_ARGS, GCC_ASM_ARGS)],
            'clean': True,
        }

def task_compile_interrupts():
    for interrupts_source in interrupts_sources:
        yield {
            'name': interrupts_source,
            'file_dep': generate_deps(interrupts_source),
            'targets': [generate_obj_path(interrupts_source)],
            'actions': ['%sgcc -o %s -c %s %s %s %s' % (GCC_TOOLS_PREFIX, generate_obj_path(interrupts_source), interrupts_source, GCC_TARGET_ARGS, GCC_ASM_ARGS, GCC_INTERRUPT_ARGS)],
            'clean': True
        }

def task_assemble_normal():
    for asm_source in asm_sources:
        yield {
            'name': asm_source,
            'file_dep': [asm_source],
            'targets': [generate_obj_path(asm_source)],
            'actions': ['nasm %s -o %s -f elf64' % (asm_source, generate_obj_path(asm_source) )],
            'clean': True,
        }

def task_assemble_bootstrap_bin():
    return {
            'file_dep': ["src/smp/smp_bootstrap.S"],
            'targets': ["src/smp/smp_bootstrap.bin"],
            'actions': ['nasm %s -o %s -f bin' % ("src/smp/smp_bootstrap.S", "src/smp/smp_bootstrap.bin")],
            'clean': True,
        }

def task_assemble_bootstrap_wrap():
    return {
            'file_dep': ["src/smp/smp_obj.S", "src/smp/smp_bootstrap.bin"],
            'targets': ["build/smp_obj.o"],
            'actions': ['nasm %s -o %s -f elf64' % ("src/smp/smp_obj.S", "build/smp_obj.o")],
            'clean': True,
        }

def task_link():
    deps = [generate_obj_path(src) for src in (c_sources + asm_sources + interrupts_sources)] + ["build/smp_obj.o"]
    return {
            'file_dep': deps,
            'targets': ["build/kernel.elf"],
            'actions': ['%sld -m elf_x86_64 -T src/link.ld %s -o %s' % (GCC_TOOLS_PREFIX, ' '.join(deps), "build/kernel.elf")],
            'clean': True,
        }
