#include "Graphics.h"

#define EFI_PLATFORM EFI_ARCH_X64
#include <efi.h>
#include <protocol/efi-gop.h>
#include <protocol/efi-lip.h>
#include <protocol/efi-sfsp.h>

// TODO: Use color when GOP is implemented
#define IF_ERROR_INTERNAL(function, message, fatal)        \
  if (EFI_STATUS status = function; status != EFI_SUCCESS) \
  {                                                        \
    Graphics::Print(L"Error: ");                           \
    Graphics::Print(message);                              \
    Graphics::Print(L": ");                                \
    Graphics::PrintLn(status);                             \
    if (fatal)                                             \
      while (true);                                        \
  }

#define IF_ERROR(function, message) IF_ERROR_INTERNAL(function, message, false)

#define IF_ERROR_FATAL(function, message) \
  IF_ERROR_INTERNAL(function, message, true)
