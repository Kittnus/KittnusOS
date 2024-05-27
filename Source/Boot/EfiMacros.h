#include "Graphics.h"

#include <efi.h>
#include <protocol/efi-gop.h>
#include <protocol/efi-lip.h>
#include <protocol/efi-sfsp.h>

// TODO: Use color when GOP is implemented
#define EFI_CHECK(function, message)                       \
  if (EFI_STATUS status = function; status != EFI_SUCCESS) \
  {                                                        \
    Graphics::SetTextColor(0xFFFFFFFF);                    \
    Graphics::SetTextBackgroundColor(0xFFFF0000);          \
    Graphics::Print(message);                              \
    Graphics::Print(": ");                                 \
    Graphics::PrintHexLn(status);                          \
    Graphics::ResetTextColor();                            \
    Graphics::ResetTextBackgroundColor();                  \
    while (true);                                          \
  }
