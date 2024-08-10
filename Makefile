SRC_DIR 		:= Source
VND_DIR		 	:= Vendor
OUT_DIR 		:= Binaries
INT_DIR			:= Intermediate

CC 					:= gcc
LD 					:= ld
AS 					:= nasm

CFLAGS	:= -Wall -Werror -ffreestanding -mno-red-zone -nostdlib -fno-stack-protector -g -fPIC -I$(SRC_DIR)/Common

DARK_GRAY 	:= \033[1;30m
LIGHT_GREEN := \033[1;32m
GREEN 			:= \033[0;32m
RESET				:= \033[0m

define print_action
	@echo "${DARK_GRAY}$1${RESET}"
endef

define print_minor_success
	@echo "${GREEN}$1${RESET}"
endef

define print_success
	@echo "${LIGHT_GREEN}$1${RESET}"
endef

all: $(OUT_DIR)/EFI/Boot/Bootx64.efi $(OUT_DIR)/Kernel.elf

BOOT_CFLAGS 	:= -I$(SRC_DIR)/Boot -I$(VND_DIR)/efi -DEFI_PLATFORM=EFI_ARCH_X64
BOOT_SECTIONS := -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc
BOOT_OBJS 		 = $(patsubst $(SRC_DIR)%.cpp,$(INT_DIR)%.o,$(wildcard $(SRC_DIR)/Boot/*.cpp))
BOOT_OBJS 		+= $(patsubst $(SRC_DIR)%.cpp,$(INT_DIR)%.o,$(wildcard $(SRC_DIR)/Boot/*/*.cpp))

$(INT_DIR)/Boot/%.o: $(SRC_DIR)/Boot/%.cpp $(wildcard $(SRC_DIR)/Boot/%.h)
	$(call print_action, "Compiling $<...")
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(BOOT_CFLAGS) -c $< -o $@
	$(call print_minor_success, "Compiled $< successfully.")

$(OUT_DIR)/EFI/Boot/Bootx64.efi: $(BOOT_OBJS)
	$(call print_action, "Converting Bootx64.so to Bootx64.efi...")
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(BOOT_CFLAGS) -shared -Wl,-dll -Wl,--subsystem,10 -e EfiMain $^ -o $@
	$(call print_success, "Converted Bootx64.so to Bootx64.efi successfully.")

KERNEL_CFLAGS	 := -I$(SRC_DIR)/Kernel
KERNEL_LDFLAGS := -O2 -g -static -fPIC -shared -Bsymbolic -nostdlib -e KernelMain
KERNEL_ASMOBJS := $(patsubst $(SRC_DIR)%.asm,$(INT_DIR)%.o,$(wildcard $(SRC_DIR)/Kernel/*.asm)) # TODO: Remvoe
KERNEL_OBJS 	  = $(patsubst $(SRC_DIR)%.cpp,$(INT_DIR)%.o,$(wildcard $(SRC_DIR)/Kernel/*.cpp))
KERNEL_OBJS 	 += $(patsubst $(SRC_DIR)%.cpp,$(INT_DIR)%.o,$(wildcard $(SRC_DIR)/Kernel/*/*.cpp))

$(INT_DIR)/Kernel/%.o: $(SRC_DIR)/Kernel/%.asm
	$(call print_action, "Assembling $<...")
	@mkdir -p $(@D)
	${AS} -f elf64 $< -o $@
	$(call print_minor_success, "Assembled $< successfully.")

$(INT_DIR)/Kernel/%.o: $(SRC_DIR)/Kernel/%.cpp $(wildcard $(SRC_DIR)/Kernel/%.h)
	$(call print_action, "Compiling $<...")
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(KERNEL_CFLAGS) -c $< -o $@
	$(call print_minor_success, "Compiled $< successfully.")

$(INT_DIR)/Kernel/Kernel.so: $(KERNEL_ASMOBJS) $(KERNEL_OBJS)
	$(call print_action, "Linking Kernel.so...")
	@mkdir -p $(@D)
	$(LD) $(KERNEL_LDFLAGS) -o $@ $^
	$(call print_success, "Linked Kernel.so successfully.")

$(OUT_DIR)/Kernel.elf: $(INT_DIR)/Kernel/Kernel.so
	$(call print_action, "Converting Kernel.so to Kernel.elf...")
	@mkdir -p $(@D)
	@objcopy -O elf64-x86-64 $< $@
	$(call print_success, "Converted Kernel.so to Kernel.elf successfully.")

QEMU_FLAGS := -L $(VND_DIR)/ovmf -bios OVMF.fd -drive file=fat:rw:$(OUT_DIR),format=raw -m 512M -kernel $(OUT_DIR)/Kernel.elf -machine type=pc-i440fx-3.1 -append nokaslr

run: all
	$(call print_action, "Running the OS with Qemu...")
	@qemu-system-x86_64 $(QEMU_FLAGS)
	$(call print_success, "Qemu exited successfully.")

debug: all
	$(call print_action, "Running the OS with Qemu in debug mode...")
	@qemu-system-x86_64 $(QEMU_FLAGS) -s -S &
	$(call print_success, "Connecting to Qemu with GDB...")
	@gdb -ex "target remote localhost:1234" -ex "symbol-file $(OUT_DIR)/EFI/Boot/Bootx64.efi" -ex "add-symbol-file $(OUT_DIR)/Kernel.elf 0x40001D0"
	$(call print_success, "Qemu exited successfully.")

clean:
	$(call print_action, "Cleaning...")
	@rm -rf $(OUT_DIR) $(INT_DIR)
	$(call print_success, "Cleaned successfully.")

.PHONY: all run clean
