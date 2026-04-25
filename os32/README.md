# Minimal 32-bit x86 Kernel (GRUB + Multiboot)

This starter project builds a **real, bootable, low-level kernel** (not a web simulation).
It is designed to boot in VirtualBox through GRUB's Multiboot support.

## 1) Bootloader Strategy (GRUB + Multiboot)

- The kernel entry assembly (`src/boot.s`) contains a **Multiboot v1 header** with:
  - magic: `0x1BADB002`
  - flags: `0x00000003` (request page alignment + memory map)
  - checksum that makes `(magic + flags + checksum) == 0`
- `grub.cfg` uses `multiboot /boot/kernel.bin` so GRUB loads the ELF kernel and jumps to `_start`.
- VirtualBox boots from the generated ISO, and GRUB handles the first stage boot process.

## 2) Kernel Entry Point (`boot.s`)

`src/boot.s` does the following:
- defines Multiboot header in `.multiboot`
- defines `_start`
- disables interrupts (`cli`)
- sets stack pointer to a static kernel stack
- calls `kmain` (C entry point)
- halts forever if `kmain` returns

## 3) Kernel in C (`kernel.c`)

`src/kernel.c` is freestanding and writes directly to VGA text mode memory:
- VGA text buffer base: `0xB8000`
- each cell: high byte color attribute, low byte ASCII character
- writes `Hello World` into the top-left of the screen

## 4) Linker Script (`linker.ld`)

The linker script:
- sets `ENTRY(_start)`
- places the kernel at `1 MiB` (`. = 1M`)
- keeps `.multiboot` in final image
- aligns sections (`.text`, `.rodata`, `.data`, `.bss`) on 4 KiB boundaries

## 5) Build + ISO Packaging

### Prerequisites (Ubuntu/Debian-style package names)

```bash
sudo apt-get update
sudo apt-get install -y build-essential nasm grub-pc-bin xorriso qemu-system-x86
```

### Build commands

```bash
cd os32
make clean
make
```

This produces:
- `build/kernel.bin`
- `build/myos.iso`

### Run quickly in QEMU (optional)

```bash
make run-qemu
```

### Boot in VirtualBox

1. Create a new VM:
   - Type: Other / Unknown
   - Memory: 64 MB is enough
2. Attach `os32/build/myos.iso` as optical media.
3. Start VM.
4. You should see `Hello World` in text mode.

## Manual build commands (without Makefile)

```bash
nasm -f elf32 src/boot.s -o build/boot.o
gcc -m32 -ffreestanding -fno-stack-protector -fno-pic -nostdlib -nostartfiles -nodefaultlibs -c src/kernel.c -o build/kernel.o
ld -m elf_i386 -T linker.ld -nostdlib -o build/kernel.bin build/boot.o build/kernel.o
mkdir -p iso/boot
auto_cp() { cp build/kernel.bin iso/boot/kernel.bin; }
auto_cp
grub-mkrescue -o build/myos.iso iso
```

## Next roadmap steps

1. Add a tiny terminal (`putchar`, newline, clear screen, cursor tracking).
2. Parse Multiboot info struct passed by GRUB.
3. Set up GDT and IDT with ISR stubs.
4. Remap PIC and handle timer/keyboard interrupts.
5. Add paging and basic physical memory manager.
6. Implement a simple heap (`kmalloc`).
7. Build a tiny shell and command dispatcher.
8. Add ATA/ATAPI or filesystem support.
9. Move toward user mode and syscall interface.
