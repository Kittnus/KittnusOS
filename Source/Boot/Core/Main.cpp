#include "Global.h"
#include "KernelLoader.h"
#include "Services/FileSystem.h"
#include "UI/Console.h"

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
  KernelLoader::Load();

  ExitBS();
  KernelLoader::Execute();
}

void DisplayCountDown()
{
  for (int i = 3; i > 0; i--)
  {
    Graphics::ClearScreen();
    Graphics::PrintLn("Kittnus Neo Bootloader - Built on " __DATE__
                      " at " __TIME__);
    Graphics::Print("Booting KittnusOS in ");

    auto character = (char)('0' + i);
    Graphics::Print(&character);

    Graphics::PrintLn(" seconds...");

    Global::BootServices->Stall(1 * 1000 * 1000);
  }

  Graphics::ClearScreen();
}

extern "C" EFI_STATUS EFIAPI efi_main(EFI_HANDLE imageHandle,
                                      EFI_SYSTEM_TABLE* systemTable)
{
  Global::Initialize(imageHandle, systemTable);
  Graphics::Initialize();

  DisplayCountDown();
  Boot();

  return EFI_SUCCESS;
}
