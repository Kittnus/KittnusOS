#include "Global.h"
#include "Graphics.h"

extern "C" EFI_STATUS EFIAPI UefiMain(EFI_HANDLE imageHandle,
                                      EFI_SYSTEM_TABLE* systemTable)
{
  Global::Initialize(imageHandle, systemTable);

  Graphics::Initialize();
  Graphics::ClearScreen();

  Graphics::Print("Hello, World!");

  return EFI_SUCCESS;
}
