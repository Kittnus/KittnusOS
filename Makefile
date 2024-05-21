SRC_DIR := Source
VND_DIR := Vendor
OUT_DIR := Binaries
INT_DIR	:= Intermediate

CC 			:= gcc
CFLAGS 	:= -Wall -Werror -m64 -mabi=ms -ffreestanding -I$(SRC_DIR)/Common

all: $(OUT_DIR)/EFI/Boot/Bootx64.efi $(OUT_DIR)/Kernel.elf

BOOT_CFLAGS := -I$(VND_DIR)/efi
BOOT_OBJS 	:= $(patsubst $(SRC_DIR)/Boot/%.cpp,$(INT_DIR)/Boot/%.o,$(wildcard $(SRC_DIR)/Boot/*.cpp))

$(INT_DIR)/Boot/%.o: $(SRC_DIR)/Boot/%.cpp $(wildcard $(SRC_DIR)/Boot/%.h)
	@echo "Compiling $<..."
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(BOOT_CFLAGS) -c $< -o $@
	@echo "Compiled $< successfully."

$(OUT_DIR)/EFI/Boot/Bootx64.efi: $(BOOT_OBJS)
	@echo "Linking Bootx64.efi..."
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(BOOT_CFLAGS) -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e UefiMain $^ -o $@
	@echo "Linked Bootx64.efi successfully."

KERNEL_CFLAGS  := 
KERNEL_ASMOBJS := $(patsubst $(SRC_DIR)/Kernel/%.asm,$(INT_DIR)/Kernel/%.o,$(wildcard $(SRC_DIR)/Kernel/*.asm))
KERNEL_OBJS 	  = $(patsubst $(SRC_DIR)/Kernel/%.cpp,$(INT_DIR)/Kernel/%.o,$(wildcard $(SRC_DIR)/Kernel/*.cpp))
KERNEL_OBJS 	 += $(patsubst $(SRC_DIR)/Kernel/%/%.cpp,$(INT_DIR)/Kernel/%/%.o,$(wildcard $(SRC_DIR)/Kernel/*/*.cpp))

$(OUT_DIR)/Kernel.elf: $(KERNEL_ASMOBJS) $(KERNEL_OBJS)
	@echo "Linking Kernel.elf..."
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -nostdlib -T $(SRC_DIR)/Kernel/Linker.ld $^ -o $@
	@echo "Linked Kernel.elf successfully."*/

clean:
	@echo "Cleaning..."
	@rm -rf $(OUT_DIR) $(INT_DIR)
	@echo "Cleaned successfully."

.PHONY: all clean
