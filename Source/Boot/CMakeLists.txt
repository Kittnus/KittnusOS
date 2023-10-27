add_executable(BOOTX64.EFI EFIMain.c)
target_compile_options(BOOTX64.EFI PRIVATE -ffreestanding)
target_link_options(BOOTX64.EFI PRIVATE -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e EfiMain)
