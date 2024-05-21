// TODO: Clean up includes
#include "Elf.h"
#include "FileSystem.h"
#include "Global.h"
#include "Graphics.h"
#include "Memory.h"
#include "StringUtils.h"

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
  {
    Graphics::PrintLn(L"Invalid ELF file");
    while (true);
  }

  for (auto i = 0ull; i < (UInt32)header->e_phentsize * header->e_phnum;
       i += header->e_phentsize)
  {
    auto programHeader =
        (Elf32ProgramHeader*)((UInt64)header + header->e_phoff + i);
    if (programHeader->p_type == PT_LOAD)
    {
      Memory::Copy((UInt8*)(UInt64)programHeader->p_vaddr,
                   (void*)((UInt64)header + programHeader->p_offset),
                   programHeader->p_filesz);
      UInt64 remaining = programHeader->p_filesz;
      while (remaining < programHeader->p_memsz)
      {
        *(UInt8*)(programHeader->p_vaddr + remaining) = 0;
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

  auto kernelFile = FileSystem::OpenFile(L"kernel.elf", EFI_FILE_MODE_READ);

  EFI_PHYSICAL_ADDRESS address = KERNEL_LOAD_ADDRESS;
  EFI_ALLOCATE_TYPE type = AllocateAddress;
  EFI_MEMORY_TYPE memoryType = EfiLoaderData;
  auto pages = 0x2000ULL; // 8 KB
  Global::BootServices->AllocatePages(type, memoryType, pages, &address);

  auto kernelSize = 0ULL;
  kernelFile->Read(kernelFile, &kernelSize, (void*)KERNEL_LOAD_ADDRESS);

  for (auto i = 0ULL; i < 0x2000; i += 4)
  {
    auto ptr = (UInt32*)(KERNEL_LOAD_ADDRESS + i);
    if (*ptr == MULTIBOOT_MAGIC)
      LoadElfKernel((Elf32Header*)KERNEL_LOAD_ADDRESS);
  }

  Graphics::PrintLn(L"Invalid kernel format, only ELF files are supported");
  while (true);
}

void FindACPITable()
{
  static const EFI_GUID acpi10TableGuid = EFI_ACPI_10_TABLE_GUID;
  static const EFI_GUID acpi20TableGuid = EFI_ACPI_20_TABLE_GUID;
  EFI_GUID acpiTableGuids[] = { acpi10TableGuid, acpi20TableGuid };
  for (auto guid : acpiTableGuids)
    for (auto j = 0ULL; j < Global::SystemTable->NumberOfTableEntries; j++)
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
            (UInt64)table->VendorTable & 0xFFFFFFFF;
    }
}

void CreateMemoryMap(MultibootHeader* header)
{
  auto mmap = (MultibootMemoryMap*)Global::KernelRoundedAddress;

  Memory::Zero((void*)Global::KernelRoundedAddress, 0x400);
  header->MmapAddr = Global::KernelRoundedAddress;

  // TODO: Error handling
  UInt64 mmapSize, mapKey, descriptorSize;
  IF_ERROR_FATAL(Global::BootServices->GetMemoryMap(&mmapSize, NULL, &mapKey,
                                                    &descriptorSize, NULL),
                 L"Failed to get memory map size");

  auto memory = (EFI_MEMORY_DESCRIPTOR*)Global::KernelRoundedAddress;
  Global::KernelEntryAddress += mmapSize;
  while ((UInt64)Global::KernelRoundedAddress & 0x3FF)
    Global::KernelRoundedAddress++;

  IF_ERROR_FATAL(Global::BootServices->GetMemoryMap(&mmapSize, memory, &mapKey,
                                                    &descriptorSize, NULL),
                 L"Failed to get memory map");

  auto upperMemory = 0ULL;
  int mmapEntries = mmapSize / descriptorSize;
  for (int i = 0; i < mmapEntries; i++)
  {
    auto descriptor =
        (EFI_MEMORY_DESCRIPTOR*)((UInt64)memory + i * descriptorSize);

    mmap->Size = descriptorSize - sizeof(UInt32);
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

    mmap = (MultibootMemoryMap*)((UInt64)mmap + mmap->Size + sizeof(UInt32));
    memory = (EFI_MEMORY_DESCRIPTOR*)((UInt64)memory + descriptorSize);
  }

  header->MmapLength = (UInt32)((UInt64)mmap - header->MmapAddr);

  header->MemLower = 0x400;
  header->MemUpper = upperMemory / 0x400;
}

#define SetMemberString(member, string)                              \
  do {                                                               \
    auto strLength = StringUtils::CalculateLength(string);           \
    Memory::Copy((void*)Global::KernelRoundedAddress, (void*)string, \
                 strLength);                                         \
    member = (UInt32)Global::KernelRoundedAddress;                   \
    Global::KernelRoundedAddress += strLength;                       \
  } while (false)

MultibootHeader* SetupMultibootHeader()
{
  auto header = (MultibootHeader*)Global::KernelRoundedAddress;
  Memory::Copy((void*)Global::KernelRoundedAddress,
               (void*)&Global::MultibootHeader, sizeof(MultibootHeader));
  Global::KernelRoundedAddress += sizeof(MultibootHeader);

  header->Flags |= MULTIBOOT_FLAGS_MEM;

  SetMemberString(header->CmdLine, L"");
  SetMemberString(header->BootLoaderName, L"Kittnus Neo");

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
  UInt64 mapKey;
  Global::BootServices->GetMemoryMap(NULL, NULL, &mapKey, NULL, NULL);
  Global::BootServices->ExitBootServices(Global::ImageHandle, mapKey);
}

void RunKernel(MultibootHeader* header)
{
  asm volatile("jmp %0"
               :
               : "r"(Global::KernelEntryAddress), "a"(MULTIBOOT_EAX_MAGIC),
                 "b"(header)
               :);

  __builtin_unreachable();
}

void Boot()
{
  // * Think about using a watchdog timer to reset the system if the kernel hangs
  // TODO: Add Boot option support

  FindACPITable();
  FileSystem::Initialize();

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
