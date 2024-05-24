SRC_DIR 		:= Source
VND_DIR		 	:= Vendor
OUT_DIR 		:= Binaries
INT_DIR			:= Intermediate

CC 					:= gcc
LD 					:= ld
AS 					:= nasm

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

CFLAGS	:= -Wall -Werror -ffreestanding -mno-red-zone -nostdlib -fno-stack-protector -fPIC -I$(SRC_DIR)/Common
LDFLAGS	:= -nostdlib -znocombreloc -shared -Bsymbolic -L /usr/lib

all: $(OUT_DIR)/EFI/Boot/Bootx64.efi $(OUT_DIR)/Kernel.elf

BOOT_CFLAGS 	:= -I$(VND_DIR)/efi -DEFI_PLATFORM=EFI_ARCH_X64
BOOT_LDFLAGS	:= -T /usr/lib/elf_x86_64_efi.lds
BOOT_SECTIONS := -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc
BOOT_OBJS 		:= $(patsubst $(SRC_DIR)%.cpp,$(INT_DIR)%.o,$(wildcard $(SRC_DIR)/Boot/*.cpp))

$(INT_DIR)/Boot/%.o: $(SRC_DIR)/Boot/%.cpp $(wildcard $(SRC_DIR)/Boot/%.h)
	$(call print_action, "Compiling $<...")
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(BOOT_CFLAGS) -c $< -o $@
	$(call print_minor_success, "Compiled $< successfully.")

$(INT_DIR)/Boot/Bootx64.so: $(BOOT_OBJS)
	$(call print_action, "Linking Bootx64.so...")
	@mkdir -p $(@D)
	$(LD) $(LDFLAGS) $(BOOT_LDFLAGS) $^ /usr/lib/crt0-efi-x86_64.o -o $@ -lefi -lgnuefi
	$(call print_minor_success, "Linked Bootx64.so successfully.")

$(OUT_DIR)/EFI/Boot/Bootx64.efi: $(INT_DIR)/Boot/Bootx64.so
	$(call print_action, "Converting Bootx64.so to Bootx64.efi...")
	@mkdir -p $(@D)
	objcopy $(BOOT_SECTIONS) --target=efi-app-x86_64 $< $@
	$(call print_success, "Converted Bootx64.so to Bootx64.efi successfully.")

KERNEL_CFLAGS  := -T $(SRC_DIR)/Kernel/Linker.ld -O2 -g -static -pedantic -mfsgsbase -fPIE -mgeneral-regs-only
KERNEL_ASMOBJS := $(patsubst $(SRC_DIR)%.asm,$(INT_DIR)%.o,$(wildcard $(SRC_DIR)/Kernel/*.asm))
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
	$(CC) $(CFLAGS) -c $< -o $@
	$(call print_minor_success, "Compiled $< successfully.")

# Won't work on Windows, you'll get the infamous PE operation on non-PE file error cuz ld sucks
$(OUT_DIR)/Kernel.elf: $(KERNEL_ASMOBJS) $(KERNEL_OBJS)
	$(call print_action, "Linking Kernel.elf...")
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(KERNEL_CFLAGS) $^ -o $@ 
	$(call print_success, "Linked Kernel.elf successfully.")

clean:
	$(call print_action, "Cleaning...")
	@rm -rf $(OUT_DIR) $(INT_DIR)
	$(call print_success, "Cleaned successfully.")

.PHONY: all clean
