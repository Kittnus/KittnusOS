#pragma once

#include "EfiMacros.h"
#include "Graphics.h"
#include "Multiboot.h"
#include "Types.h"

class Global
{
public:
  static inline MultibootHeader MultibootHeader;

  static inline UInt64 KernelEntryAddress;
  static inline UInt64 KernelRoundedAddress;

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
