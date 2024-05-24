SRC_DIR 		:= Source
VND_DIR		 	:= Vendor
OUT_DIR 		:= Binaries
INT_DIR			:= Intermediate

CC 					:= gcc
LD 					:= ld
AS 					:= nasm

CFLAGS	:= -Wall -Werror -ffreestanding -mno-red-zone -nostdlib -fno-stack-protector -fPIC -I$(SRC_DIR)/Common

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

all: $(OUT_DIR)/EFI/Boot/Bootx64.efi $(OUT_DIR)/Kernel.bin

BOOT_CFLAGS 	:= -I$(VND_DIR)/efi -DEFI_PLATFORM=EFI_ARCH_X64
BOOT_SECTIONS := -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc
BOOT_OBJS 		:= $(patsubst $(SRC_DIR)%.cpp,$(INT_DIR)%.o,$(wildcard $(SRC_DIR)/Boot/*.cpp))

$(INT_DIR)/Boot/%.o: $(SRC_DIR)/Boot/%.cpp $(wildcard $(SRC_DIR)/Boot/%.h)
	$(call print_action, "Compiling $<...")
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(BOOT_CFLAGS) -c $< -o $@
	$(call print_minor_success, "Compiled $< successfully.")

$(OUT_DIR)/EFI/Boot/Bootx64.efi: $(BOOT_OBJS)
	$(call print_action, "Converting Bootx64.so to Bootx64.efi...")
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(BOOT_CFLAGS) -shared -Wl,-dll -Wl,--subsystem,10 -e efi_main $^ -o $@
	$(call print_success, "Converted Bootx64.so to Bootx64.efi successfully.")

KERNEL_LDFLAGS  := -O2 -g -static -fPIC -shared -Bsymbolic -nostdlib -e start
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

$(INT_DIR)/Kernel/Kernel.so: $(KERNEL_ASMOBJS) $(KERNEL_OBJS)
	$(call print_action, "Linking Kernel.so...")
	@mkdir -p $(@D)
	$(LD) $(KERNEL_LDFLAGS) -o $@ $^
	$(call print_success, "Linked Kernel.so successfully.")

$(OUT_DIR)/Kernel.bin: $(INT_DIR)/Kernel/Kernel.so
	$(call print_action, "Linking Kernel.bin...")
	@mkdir -p $(@D)
	objcopy -O binary $< $@
	$(call print_success, "Linked Kernel.bin successfully.")

clean:
	$(call print_action, "Cleaning...")
	@rm -rf $(OUT_DIR) $(INT_DIR)
	$(call print_success, "Cleaned successfully.")

.PHONY: all clean
