#include "FileSystem.h"
#include "Global.h"
#include "Graphics.h"
#include "Kernel.h"

void ExitBS()
{
  UInt64 mapKey;
  Global::BootServices->GetMemoryMap(NULL, NULL, &mapKey, NULL, NULL);
  Global::BootServices->ExitBootServices(Global::ImageHandle, mapKey);
}

void Boot()
{
  // TODO: Think about using a watchdog timer to reset the system if the loader hangs
  // TODO: Add Boot option support
  FileSystem::Initialize();
  Kernel::Initialize();

  ExitBS();
  Kernel::Execute();
}

void DisplayCountDown()
{
  // TODO: Make this look better once Graphics uses GOP
  for (int i = 3; i > 0; i--)
  {
    Graphics::ClearScreen();
    Graphics::Print(L"Booting KittnusOS in ");

    auto character = (wchar_t)(L'0' + i);
    Graphics::Print(&character);

    Graphics::PrintLn(L" seconds...");

    Global::BootServices->Stall(1 * 1000 * 1000);
  }

  Graphics::ClearScreen();
}

extern "C" EFI_STATUS EFIAPI efi_main(EFI_HANDLE imageHandle,
                                      EFI_SYSTEM_TABLE* systemTable)
{
  Global::Initialize(imageHandle, systemTable);

  Graphics::Initialize();
  Graphics::ClearScreen();

  DisplayCountDown();
  Boot();

  return EFI_SUCCESS;
}
