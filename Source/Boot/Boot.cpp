#include "Global.h"
#include "Graphics.h"

void LoadKernel()
{
#define KERNEL_LOAD_ADDRESS 0x4000000ULL // 64 MB offset

  EFI_FILE_PROTOCOL* kernelFile;
  Global::RootDirectory->Open(Global::RootDirectory, &kernelFile,
                              (CHAR16*)L"Kernel.elf", EFI_FILE_MODE_READ, 0);

  EFI_PHYSICAL_ADDRESS address = KERNEL_LOAD_ADDRESS;
  EFI_ALLOCATE_TYPE type = AllocateAddress;
  EFI_MEMORY_TYPE memoryType = EfiLoaderData;
  UINTN pages = 0x2000; // 8 KB
  Global::BootServices->AllocatePages(type, memoryType, pages, &address);

  UINTN kernelSize = 0;
  kernelFile->Read(kernelFile, &kernelSize, (void*)KERNEL_LOAD_ADDRESS);

  for (UINTN i = 0; i < 0x2000; i += 4)
  {
    auto ptr = (UINT32*)(KERNEL_LOAD_ADDRESS + i);
    if (*ptr == MULTIBOOT_MAGIC)
    {
      auto kernelHeader = (Elf32Header*)KERNEL_LOAD_ADDRESS;
      (void)kernelHeader;

      break;
    }
  }

  // TODO: Throw error when no multiboot header is found
}

void Boot()
{
  // * Think about using a watchdog timer to reset the system if the kernel hangs
  // TODO: Get ACPI tables
  // TODO: Add Boot option support

  // TODO: Implement error handling
  EFI_GUID loadedImageProtocolGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
  Global::BootServices->HandleProtocol(Global::ImageHandle,
                                       &loadedImageProtocolGuid,
                                       (void**)&Global::LoadedImage);

  EFI_GUID simpleFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
  Global::BootServices->HandleProtocol(Global::LoadedImage->DeviceHandle,
                                       &simpleFileSystemProtocolGuid,
                                       (void**)&Global::SimpleFileSystem);

  Global::SimpleFileSystem->OpenVolume(Global::SimpleFileSystem,
                                       &Global::RootDirectory);

  LoadKernel();
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
