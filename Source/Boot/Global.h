#pragma once

#define EFI_PLATFORM EFI_ARCH_X64
#include <cstddef>
#include <efi.h>

class Global {
public:
  static void Initialize(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);

  static EFI_SYSTEM_TABLE* SystemTable;
  static EFI_HANDLE ImageHandle;

  static EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut;
  static EFI_SIMPLE_TEXT_INPUT_PROTOCOL* ConIn;
};
