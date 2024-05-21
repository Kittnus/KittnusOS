#pragma once

#define NULL 0

#define EFI_PLATFORM EFI_ARCH_X64
#include <efi.h>
#include <protocol/efi-gop.h>
#include <protocol/efi-lip.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <protocol/efi-sfsp.h> // Fix the flexible array member issue in EFI_FILE_SYSTEM_VOLUME_LABEL
#pragma GCC diagnostic pop

#include "Elf.h"
#include "Multiboot.h"

class Global
{
public:
  static inline MultibootHeader MultibootHeader;

  static inline EFI_SYSTEM_TABLE* SystemTable;
  static inline EFI_HANDLE ImageHandle;

  static inline EFI_BOOT_SERVICES* BootServices;

  static inline EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut;
  static inline EFI_SIMPLE_TEXT_INPUT_PROTOCOL* ConIn;

  static inline EFI_GRAPHICS_OUTPUT_PROTOCOL* GraphicsOutput;

  static inline EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
  static inline EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* SimpleFileSystem;
  static inline EFI_FILE_PROTOCOL* RootDirectory;

public:
  static void Initialize(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable);
};
