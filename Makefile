CC 		:= gcc
CFLAGS 	:=-Wall -Werror -m64 -mabi=ms

OUTPUT_DIR := Binaries

EFI_OUTPUT_PATH := $(OUTPUT_DIR)/BOOTX64.efi

all: BOOTX64.efi

%.o: %.c:
	$(CC) $(CFLAGS) -I -ffreestanding -c $< -o $@

EFI_OUTPUT_PATH: Source/Boot/EFIMain.o
	$(CC) $(CFLAGS) -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e EfiMain $^ -o $@

clean:
	rm -f Source/*.o
	rm -f EFI_OUTPUT_PATH

.PHONY: all clean