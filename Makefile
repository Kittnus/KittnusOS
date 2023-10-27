SRC_DIR := Source
OUT_DIR := Binaries
INT_DIR	:= Intermediate

CC 		:= gcc
CFLAGS 	:= -Wall -Werror -m64 -mabi=ms -ffreestanding -I$(SRC_DIR)

SRCS := $(wildcard $(SRC_DIR)/**/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(INT_DIR)/%.o, $(SRCS))

all: $(OBJS) $(OUT_DIR)/BOOTX64.efi

$(INT_DIR):
	@mkdir -p $@

$(OUT_DIR):
	@mkdir -p $@

$(INT_DIR)/%.o: $(SRC_DIR)/%.c | $(INT_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/BOOTX64.efi: $(INT_DIR)/Boot/EFIMain.o | $(OUT_DIR)
	$(CC) $(CFLAGS) -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e EfiMain $^ -o $@

clean:
	rm -rf $(OUT_DIR) $(INT_DIR)

.PHONY: all clean