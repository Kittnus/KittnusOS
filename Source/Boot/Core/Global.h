#pragma once

#include "Types.h"
#include "UI/Graphics.h"
#include "Utilities/EfiMacros.h"

class Global
{
public:
  static inline EFI_SYSTEM_TABLE* SystemTable;
  static inline EFI_HANDLE ImageHandle;

  static inline EFI_BOOT_SERVICES* BootServices;

  static inline EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut;
  static inline EFI_SIMPLE_TEXT_INPUT_PROTOCOL* ConIn;

public:
  static void Initialize(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable);
};
