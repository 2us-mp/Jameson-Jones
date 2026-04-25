ARCH := i386
CC := gcc
LD := ld
NASM := nasm

CFLAGS := -m32 -ffreestanding -fno-stack-protector -fno-pic -nostdlib -nostartfiles -nodefaultlibs -Wall -Wextra
LDFLAGS := -m elf_i386 -T linker.ld -nostdlib

BUILD_DIR := build
ISO_DIR := iso
SRC_DIR := src

all: $(BUILD_DIR)/myos.iso

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/boot.o: $(SRC_DIR)/boot.s | $(BUILD_DIR)
	$(NASM) -f elf32 $< -o $@

$(BUILD_DIR)/kernel.o: $(SRC_DIR)/kernel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o linker.ld
	$(LD) $(LDFLAGS) -o $@ $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o

$(BUILD_DIR)/myos.iso: $(BUILD_DIR)/kernel.bin $(ISO_DIR)/boot/grub/grub.cfg
	mkdir -p $(ISO_DIR)/boot
	cp $(BUILD_DIR)/kernel.bin $(ISO_DIR)/boot/kernel.bin
	grub-mkrescue -o $@ $(ISO_DIR)

run-qemu: $(BUILD_DIR)/myos.iso
	qemu-system-i386 -cdrom $(BUILD_DIR)/myos.iso

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run-qemu clean
