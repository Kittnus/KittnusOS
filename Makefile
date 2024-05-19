SRC_DIR := Source
VND_DIR := Vendor
OUT_DIR := Binaries
INT_DIR	:= Intermediate

CC 		:= gcc
CFLAGS 	:= -Wall -Werror -m64 -mabi=ms -ffreestanding -I$(SRC_DIR) -I$(VND_DIR)/efi

SRCS := $(wildcard $(SRC_DIR)/**/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(INT_DIR)/%.o, $(SRCS))

all: $(OBJS) $(OUT_DIR)/BOOTX64.efi

$(INT_DIR):
	@mkdir $@

$(OUT_DIR):
	@mkdir $@

$(INT_DIR)/%.o: $(SRC_DIR)/%.cpp | $(INT_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/BOOTX64.efi: $(INT_DIR)/Boot/Boot.o | $(OUT_DIR)
	$(CC) $(CFLAGS) -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e UefiMain $^ -o $@

clean:
	rm -rf $(OUT_DIR) $(INT_DIR)

.PHONY: all clean