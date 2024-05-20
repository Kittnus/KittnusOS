#include "Global.h"
#include "Graphics.h"

void Boot()
{
  // * Think about using a watchdog timer to reset the system if the kernel hangs
  // TODO: Get ACPI tables

  
}

void CountDown()
{
  // TODO: Make this look better once Graphics uses GOP
  for (int i = 5; i > 0; i--)
  {
    Graphics::ClearScreen();
    Graphics::Print(L"Booting KittnusOS in ");

    auto character = (wchar_t)(L'0' + i);
    Graphics::Print(&character);

    Graphics::PrintLn(L" seconds...");

    Global::BootServices->Stall(1 * 1000 * 1000);
  }
}

extern "C" EFI_STATUS EFIAPI UefiMain(EFI_HANDLE imageHandle,
                                      EFI_SYSTEM_TABLE* systemTable)
{
  Global::Initialize(imageHandle, systemTable);

  Graphics::Initialize();
  Graphics::ClearScreen();

  CountDown();
  Boot();

  return EFI_SUCCESS;
}
