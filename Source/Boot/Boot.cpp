#include "Global.h"
#include "Graphics.h"

extern "C" EFI_STATUS EFIAPI UefiMain(EFI_HANDLE ImageHandle,
                                      EFI_SYSTEM_TABLE* SystemTable)
{
  Global::Initialize(ImageHandle, SystemTable);

  Graphics::ClearScreen();

  return EFI_SUCCESS;
}
