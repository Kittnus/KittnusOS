#include "Global.h"
#include "Graphics.h"

// TODO: Make a fucking memcpy function
// TODO: Organize methods into files
void RealignMemory()
{
  Global::KernelRoundedAddress =
      (Global::KernelRoundedAddress & ~0xFFF)
      + ((Global::KernelRoundedAddress & 0xFFF) ? 0x1000 : 0);
}

void LoadElfKernel(Elf32Header* header)
{
  if ((header->e_ident[0] != ELFMAG0 || header->e_ident[1] != ELFMAG1
       || header->e_ident[2] != ELFMAG2 || header->e_ident[3] != ELFMAG3)
      || header->e_type != ET_EXEC)
    return; // TODO: Throw error

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
      if (programHeader->p_vaddr + programHeader->p_memsz
          > Global::KernelRoundedAddress)
        Global::KernelRoundedAddress = programHeader->p_vaddr;
    }
  }

  Global::KernelEntryAddress = header->e_entry;
  RealignMemory();
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
      LoadElfKernel((Elf32Header*)KERNEL_LOAD_ADDRESS);
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

void CreateMemoryMap(MultibootHeader* header)
{
  auto mmap = (MultibootMemoryMap*)Global::KernelRoundedAddress;
  for (auto i = 0; i < 1024; i++)
    *(UINT8*)(Global::KernelRoundedAddress + i) = 0;
  header->MmapAddr = Global::KernelRoundedAddress;

  // TODO: Error handling
  UINTN mmapSize, mapKey, descriptorSize;
  Global::BootServices->GetMemoryMap(&mmapSize, NULL, &mapKey, &descriptorSize,
                                     NULL);

  auto memory = (EFI_MEMORY_DESCRIPTOR*)Global::KernelRoundedAddress;
  Global::KernelEntryAddress += mmapSize;
  while ((UINTN)Global::KernelRoundedAddress & 0x3FF)
    Global::KernelRoundedAddress++;

  Global::BootServices->GetMemoryMap(&mmapSize, memory, &mapKey,
                                     &descriptorSize, NULL);

  UINTN upperMemory = 0;
  int mmapEntries = mmapSize / descriptorSize;
  for (int i = 0; i < mmapEntries; i++)
  {
    auto descriptor =
        (EFI_MEMORY_DESCRIPTOR*)((UINTN)memory + i * descriptorSize);

    mmap->Size = descriptorSize - sizeof(UINT32);
    mmap->BaseAddr = descriptor->PhysicalStart;
    mmap->Length = descriptor->NumberOfPages * 4096;

    switch (descriptor->Type)
    {
    case EfiConventionalMemory:
    case EfiLoaderCode:
    case EfiLoaderData:
    case EfiBootServicesCode:
    case EfiBootServicesData:
    case EfiRuntimeServicesCode:
    case EfiRuntimeServicesData:
      mmap->Type = 1; // Available memory
      break;
    case EfiReservedMemoryType:
    case EfiUnusableMemory:
    case EfiMemoryMappedIO:
    case EfiMemoryMappedIOPortSpace:
    case EfiPalCode:
    case EfiACPIMemoryNVS:
    case EfiACPIReclaimMemory:
      mmap->Type = 2; // Reserved memory
      break;
    }

    if (mmap->Type == 1 && mmap->BaseAddr >= 0x100000)
      upperMemory += mmap->Length;

    mmap = (MultibootMemoryMap*)((UINTN)mmap + mmap->Size + sizeof(UINT32));
    memory = (EFI_MEMORY_DESCRIPTOR*)((UINTN)memory + descriptorSize);
  }

  header->MmapLength = (UINT32)((UINTN)mmap - header->MmapAddr);

  header->MemLower = 0x400;
  header->MemUpper = upperMemory / 0x400;
}

MultibootHeader* SetupMultibootHeader()
{
  auto header = (MultibootHeader*)Global::KernelRoundedAddress;
  for (UINTN i = 0; i < sizeof(MultibootHeader); i++)
    *(UINT8*)(Global::KernelRoundedAddress + i) =
        *(UINT8*)(&Global::MultibootHeader + i);
  Global::KernelRoundedAddress += sizeof(MultibootHeader);

  header->Flags |= MULTIBOOT_FLAGS_MMAP;

  auto cmdLine = "";
  for (UINTN i = 0; i < sizeof(cmdLine) + 1; i++)
    *(UINT8*)(Global::KernelRoundedAddress + i) = cmdLine[i];
  header->CmdLine = Global::KernelRoundedAddress;
  Global::KernelRoundedAddress += sizeof(cmdLine) + 1;

  auto name = "Kitten Loader";
  for (UINTN i = 0; i < sizeof(name) + 1; i++)
    *(UINT8*)(Global::KernelRoundedAddress + i) = name[i];
  header->BootLoaderName = Global::KernelRoundedAddress;
  Global::KernelRoundedAddress += sizeof(name) + 1;

  auto graphicsMode = Global::GraphicsOutput->Mode;
  auto graphicsInfo = graphicsMode->Info;
  header->FramebufferAddr = graphicsMode->FrameBufferBase;
  header->FramebufferPitch = graphicsInfo->PixelsPerScanLine * 4;
  header->FramebufferWidth = graphicsInfo->HorizontalResolution;
  header->FramebufferHeight = graphicsInfo->VerticalResolution;
  header->FramebufferBpp = 32;

  RealignMemory();

  return header;
}

void ExitBootServices()
{
  UINTN mapKey;
  Global::BootServices->GetMemoryMap(NULL, NULL, &mapKey, NULL, NULL);
  Global::BootServices->ExitBootServices(Global::ImageHandle, mapKey);
}

void RunKernel(MultibootHeader* header)
{
  __asm__ __volatile__("jmp %0" ::"r"(Global::KernelEntryAddress),
                       "a"(MULTIBOOT_EAX_MAGIC), "b"(header));

  __builtin_unreachable();
}

void Boot()
{
  // * Think about using a watchdog timer to reset the system if the kernel hangs
  // TODO: Add Boot option support

  FindACPITable();
  SetupFileSystem();

  LoadKernel();

  auto header = SetupMultibootHeader();

  CreateMemoryMap(header);

  ExitBootServices();
  RunKernel(header);
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
