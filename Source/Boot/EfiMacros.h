#include "Graphics.h"

#define EFI_PLATFORM EFI_ARCH_X64
#include <efi.h>
#include <protocol/efi-gop.h>
#include <protocol/efi-lip.h>
#include <protocol/efi-sfsp.h>

// TODO: Use color when GOP is implemented
#define IF_ERROR(function, message, isFatal)               \
  if (EFI_STATUS status = function; status != EFI_SUCCESS) \
  {                                                        \
    Graphics::Print(L"Error: ");                           \
    Graphics::Print(message);                              \
    Graphics::Print(L": ");                                \
    Graphics::PrintLn(status);                             \
    if (isFatal)                                           \
      while (true);                                        \
  }
