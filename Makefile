SRC_DIR := Source
VND_DIR := Vendor
OUT_DIR := Binaries
INT_DIR	:= Intermediate

CC 			:= gcc
CFLAGS 	:= -Wall -Werror -m64 -mabi=ms -ffreestanding -I$(SRC_DIR)/Common

all: $(OUT_DIR)/EFI/Boot/Bootx64.efi

BOOT_INCLUDES = -I$(VND_DIR)/efi
BOOT_OBJS := $(patsubst $(SRC_DIR)/Boot/%.cpp,$(INT_DIR)/Boot/%.o,$(wildcard $(SRC_DIR)/Boot/*.cpp))

$(INT_DIR)/Boot/%.o: $(SRC_DIR)/Boot/%.cpp $(wildcard $(SRC_DIR)/Boot/%.h)
	@echo "Compiling $<..."
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(BOOT_INCLUDES) -c $< -o $@
	@echo "Compiled $< successfully."

$(OUT_DIR)/EFI/Boot/Bootx64.efi: $(BOOT_OBJS)
	@echo "Linking Bootx64.efi..."
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(BOOT_INCLUDES) -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e UefiMain $^ -o $@
	@echo "Linked Bootx64.efi successfully."

# $(OUT_DIR)/Kernel

clean:
	@echo "Cleaning..."
	@rm -rf $(OUT_DIR) $(INT_DIR)
	@echo "Cleaned successfully."

.PHONY: all clean
