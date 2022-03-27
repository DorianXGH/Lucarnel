import os
import pconf
from powerliner import *
from powerliner import tasker
from doit.reporter import ConsoleReporter

GCC_ASM_ARGS = "-masm=intel"
GAS_ASM_ARGS = "-msyntax=intel -mnaked-reg"
GCC_TARGET_ARGS = "-ffreestanding -mno-red-zone -std=c11 -Werror -fcompare-debug-second -mcmodel=large -ggdb"
GCC_INTERRUPT_ARGS = "-mgeneral-regs-only"
GCC_TOOLS_PREFIX = "/home/dorian/opt/cross/bin/" + "x86_64-elf-"

c_sources = [
    "src/main.c",
    "src/video/video.c",
    "src/memory/paging.c",
    "src/utils.c",
    "src/interrupts/apic.c",
    "src/acpi.c",
    "src/pci.c",
    "src/memory/mmap.c",
    "src/video/cons.c"
]
interrupts_sources = [
    "src/interrupts/ISR.c",
    "src/interrupts/IRQ.c",
    "src/interrupts/IDT.c",
]

asm_sources = [
    "src/entry.S",
    "src/x86_64_utils.S",
    "src/interrupts/IRQ.S"
]


class MyReporter(ConsoleReporter):
    def splitter(self, task):
        title = task.title()
        handle_file = str(title).split(":", 1)
        if(len(handle_file) == 1):
            return [" "+handle_file[0]+" ", ""]
        else:
            return [" "+handle_file[0]+" ", handle_file[1]]

    def execute_task(self, task):
        split_title = self.splitter(task)
        self.write(PowerlineSequence(
            [["Black", "Grey", split_title[0]]] +
            tasker(split_title[1], "..."), True).render(["Right"])+"\r")

    def add_failure(self, task, exception):
        """called when execution finishes with a failure"""
        split_title = self.splitter(task)
        self.write("\33[2K"+PowerlineSequence([["Black", "Grey", split_title[0]]] +
                                              tasker(split_title[1], "ERROR", exception.get_name()), True).render(["Right"])+"\r\n")
        result = {'task': task, 'exception': exception}
        self.failures.append(result)
        self._write_failure(result)

    def add_success(self, task):
        """called when execution finishes successfully"""
        split_title = self.splitter(task)
        self.write("\33[2K"+PowerlineSequence(
            [["Black", "Grey", split_title[0]]] +
            tasker(split_title[1], "OK"), True).render(["Right"])+"\r\n")

    def skip_uptodate(self, task):
        """skipped up-to-date task"""
        split_title = self.splitter(task)
        if task.name[0] != '_':
            self.write(PowerlineSequence([["Black", "Grey", split_title[0]]] +
                                         tasker(split_title[1], "SKIPPED"), True).render(["Right"])+"\r\n")

    def skip_ignore(self, task):
        """skipped ignored task"""
        split_title = self.splitter(task)
        self.write(PowerlineSequence(
            [["Black", "Grey", split_title[0]]] +
            tasker(split_title[1], "IGNORED"), True).render(["Right"])+"\r\n")


DOIT_CONFIG = {'reporter': MyReporter}


def generate_deps(c_source):
    stream = os.popen(GCC_TOOLS_PREFIX + "gcc -MM " + c_source)
    output = stream.read()
    deps = output.split(' ')
    real_deps = [dep.strip("\n") for dep in deps if (
        not dep.endswith(".o:") and dep != "\\\n")]
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
            'actions': ['nasm %s -o %s -f elf64' % (asm_source, generate_obj_path(asm_source))],
            'clean': True,
        }

def task_link():
    deps = [generate_obj_path(src) for src in (
        c_sources + asm_sources + interrupts_sources)]
    return {
        'file_dep': deps+["src/link.ld"],
        'targets': ["build/kernel.elf"],
        'actions': ['%sld -m elf_x86_64 -T src/link.ld %s -o %s' % (GCC_TOOLS_PREFIX, ' '.join(deps), "build/kernel.elf")],
        'clean': True,
    }
