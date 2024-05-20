#pragma once

#define NULL 0

#define EFI_PLATFORM EFI_ARCH_X64
#include <efi.h>
#include <protocol/efi-gop.h>

class Global
{
public:
  static void Initialize(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable);

  static inline EFI_SYSTEM_TABLE* SystemTable;
  static inline EFI_HANDLE ImageHandle;

  static inline EFI_BOOT_SERVICES* BootServices;

  static inline EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut;
  static inline EFI_SIMPLE_TEXT_INPUT_PROTOCOL* ConIn;

  static inline EFI_GRAPHICS_OUTPUT_PROTOCOL* GraphicsOutput;
};
