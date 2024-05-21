#include "Global.h"
#include "Graphics.h"

void LoadElfKernel(Elf32Header* header)
{
  if ((header->e_ident[0] != ELFMAG0 || header->e_ident[1] != ELFMAG1
       || header->e_ident[2] != ELFMAG2 || header->e_ident[3] != ELFMAG3)
      || header->e_type != ET_EXEC)
    return; // TODO: Throw error

  UINTN entry = header->e_entry;
  for (UINTN i = 0; i < (UINT32)header->e_phentsize * header->e_phnum;
       i += header->e_phentsize)
  {
    auto programHeader =
        (Elf32ProgramHeader*)((UINTN)header + header->e_phoff + i);
    if (programHeader->p_type == PT_LOAD)
    {
      for (UINTN j = 0; j < programHeader->p_filesz; j++)
        *(UINT8*)(programHeader->p_vaddr + j) =
            *(UINT8*)((UINTN)header + programHeader->p_offset + j);
      UINTN remaining = programHeader->p_filesz;
      while (remaining < programHeader->p_memsz)
      {
        *(UINT8*)(programHeader->p_vaddr + remaining) = 0;
        remaining++;
      }
      if (programHeader->p_vaddr + programHeader->p_memsz > entry)
        entry = programHeader->p_vaddr;
    }
  }

  Global::KernelEntryAddress = entry;
}

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
      LoadElfKernel((Elf32Header*)KERNEL_LOAD_ADDRESS);
    }
  }

  // TODO: Throw error when no multiboot header is found
}

void FindACPITable()
{
  static const EFI_GUID acpi10TableGuid =
      EFO_ACPI_10_TABLE_GUID; // Typo in the original code hahaha
  static const EFI_GUID acpi20TableGuid = EFI_ACPI_20_TABLE_GUID;
  EFI_GUID acpiTableGuids[] = { acpi10TableGuid, acpi20TableGuid };
  for (auto guid : acpiTableGuids)
    for (UINTN j = 0; j < Global::SystemTable->NumberOfTableEntries; j++)
    {
      auto table = &Global::SystemTable->ConfigurationTable[j];
      if (table->VendorGuid.Data1 == guid.Data1
          && table->VendorGuid.Data2 == guid.Data2
          && table->VendorGuid.Data3 == guid.Data3
          && table->VendorGuid.Data4[0] == guid.Data4[0]
          && table->VendorGuid.Data4[1] == guid.Data4[1]
          && table->VendorGuid.Data4[2] == guid.Data4[2]
          && table->VendorGuid.Data4[3] == guid.Data4[3]
          && table->VendorGuid.Data4[4] == guid.Data4[4]
          && table->VendorGuid.Data4[5] == guid.Data4[5]
          && table->VendorGuid.Data4[6] == guid.Data4[6]
          && table->VendorGuid.Data4[7] == guid.Data4[7])
        Global::MultibootHeader.ConfigTable =
            (UINTN)table->VendorTable & 0xFFFFFFFF;
    }
}

void SetupFileSystem()
{
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
}

void Boot()
{
  // * Think about using a watchdog timer to reset the system if the kernel hangs
  // TODO: Add Boot option support

  FindACPITable();
  SetupFileSystem();
  LoadKernel();
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
}

extern "C" EFI_STATUS EFIAPI UefiMain(EFI_HANDLE imageHandle,
                                      EFI_SYSTEM_TABLE* systemTable)
{
  Global::Initialize(imageHandle, systemTable);

  Graphics::Initialize();
  Graphics::ClearScreen();

  DisplayCountDown();
  Boot();

  return EFI_SUCCESS;
}
