#include "Global.h"
#include "Graphics.h"

extern "C" EFI_STATUS EFIAPI UefiMain(EFI_HANDLE imageHandle,
                                      EFI_SYSTEM_TABLE* systemTable)
{
  Global::Initialize(imageHandle, systemTable);

  Graphics::Initialize();
  Graphics::ClearScreen();

  Graphics::Print(L"Hello, World!");

  Global::BootServices->Stall(30 * 1000 * 1000); // 30 seconds

  return EFI_SUCCESS;
}
