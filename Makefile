SRC_DIR := Source
VND_DIR := Vendor
OUT_DIR := Binaries
INT_DIR	:= Intermediate

CC 		:= gcc
CFLAGS 	:= -Wall -Werror -m64 -mabi=ms -ffreestanding -I$(SRC_DIR) -I$(VND_DIR)/efi

all: $(OUT_DIR)/BOOTX64.efi

$(OUT_DIR):
	mkdir -p $@

BOOT_OBJS := $(patsubst $(SRC_DIR)/Boot/%.cpp,$(INT_DIR)/Boot/%.o,$(wildcard $(SRC_DIR)/Boot/*.cpp))

$(INT_DIR)/Boot/%.o: $(SRC_DIR)/Boot/%.cpp $(wildcard $(SRC_DIR)/Boot/%.h)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/BOOTX64.efi: $(BOOT_OBJS) | $(OUT_DIR)
	$(CC) $(CFLAGS) -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e UefiMain $^ -o $@

clean:
	rm -rf $(OUT_DIR) $(INT_DIR)

.PHONY: all clean
