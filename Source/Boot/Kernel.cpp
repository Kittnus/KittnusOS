#include "Kernel.h"

#include "FileSystem.h"
#include "Global.h"
#include "Memory.h"
#include "String.h"

#define SET_MB_MEMBER_STRING(member, string)                           \
  do {                                                                 \
    auto strLength = String::CalculateLength(string);                  \
    Memory::Copy((void*)s_KernelEndRounded, (void*)string, strLength); \
    member = (UInt32)s_KernelEndRounded;                               \
    s_KernelEndRounded += strLength;                                   \
  } while (false)

void Kernel::FindACPI()
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
        s_Header.ConfigTable = (UInt64)table->VendorTable & 0xFFFFFFFF;
    }
}

void Kernel::Initialize()
{
  Load();

  InitializeMultiboot();
  SetupMultiboot();

  CreateMemoryMap();
}

void Kernel::Execute()
{
  typedef void (*KernelEntry)(UInt32, UInt32);
  ((KernelEntry)s_KernelEntry)(
      MULTIBOOT_EAX_MAGIC,
      (UInt32)(UInt64)&s_Header); // TODO: Check if this actually works lol

  while (true);
}

void Kernel::RealignMemory()
{
  s_KernelEndRounded = (s_KernelEndRounded & ~0xFFF)
                       + ((s_KernelEndRounded & 0xFFF) ? 0x1000 : 0);
}

void Kernel::Load()
{
  constexpr UInt64 KERNEL_LOAD_ADDRESS = 0x4000000ULL; // 64 MB offset

  auto kernelFile = FileSystem::OpenFile(L"Kernel.elf", EFI_FILE_MODE_READ);

  EFI_PHYSICAL_ADDRESS address = KERNEL_LOAD_ADDRESS;
  EFI_ALLOCATE_TYPE type = AllocateAddress;
  EFI_MEMORY_TYPE memoryType = EfiLoaderData;
  auto pages = 0x2000ULL; // 8 KiB
  Global::BootServices->AllocatePages(type, memoryType, pages, &address);

  auto kernelSize = 0ULL;
  kernelFile->Read(kernelFile, &kernelSize, (void*)KERNEL_LOAD_ADDRESS);

  for (auto i = 0ULL; i < 0x2000; i += 4)
  {
    auto ptr = (UInt32*)(KERNEL_LOAD_ADDRESS + i);
    if (*ptr == MULTIBOOT_MAGIC) LoadElf((Elf32Header*)KERNEL_LOAD_ADDRESS);
  }

  Graphics::PrintLn(L"Invalid kernel format");
  while (true);
}

void Kernel::LoadElf(Elf32Header* elfHeader)
{
  if ((elfHeader->e_ident[0] != ELFMAG0 || elfHeader->e_ident[1] != ELFMAG1
       || elfHeader->e_ident[2] != ELFMAG2 || elfHeader->e_ident[3] != ELFMAG3)
      || elfHeader->e_type != ET_EXEC)
  {
    Graphics::PrintLn(L"Invalid ELF file");
    while (true);
  }

  for (auto i = 0ull; i < (UInt32)elfHeader->e_phentsize * elfHeader->e_phnum;
       i += elfHeader->e_phentsize)
  {
    auto programHeader =
        (Elf32ProgramHeader*)((UInt64)elfHeader + elfHeader->e_phoff + i);
    if (programHeader->p_type == PT_LOAD)
    {
      Memory::Copy((UInt8*)(UInt64)programHeader->p_vaddr,
                   (void*)((UInt64)elfHeader + programHeader->p_offset),
                   programHeader->p_filesz);
      UInt64 remaining = programHeader->p_filesz;
      while (remaining < programHeader->p_memsz)
      {
        *(UInt8*)(programHeader->p_vaddr + remaining) = 0;
        remaining++;
      }
      if (programHeader->p_vaddr + programHeader->p_memsz > s_KernelEndRounded)
        s_KernelEndRounded = programHeader->p_vaddr;
    }
  }

  s_KernelEntry = elfHeader->e_entry;
  RealignMemory();
}

void Kernel::InitializeMultiboot()
{
  s_Header = {
    MULTIBOOT_FLAGS_MEM | MULTIBOOT_FLAGS_CMDLINE | MULTIBOOT_FLAGS_MMAP
        | MULTIBOOT_FLAGS_LOADER_NAME, // Flags

    0x100000, // MemLower
    0x640000, // MemUpper

    0, // BootDevice

    0, // CmdLine

    0, // ModsCount
    0, // ModsAddr

    0, // Num
    0, // Size
    0, // Addr
    0, // Shndx

    0, // MmapLength
    0, // MmapAddr

    0, // DrivesLength
    0, // DrivesAddr

    0, // ConfigTable

    0, // BootLoaderName

    0, // ApmTable

    0, // VbeControlInfo
    0, // VbeModeInfo
    0, // VbeMode
    0, // VbeInterfaceSeg
    0, // VbeInterfaceOff
    0, // VbeInterfaceLen

    0, // FramebufferAddr
    0, // FramebufferPitch
    0, // FramebufferWidth
    0, // FramebufferHeight
    0, // FramebufferBpp
    0, // FramebufferType
  };
}

void Kernel::SetupMultiboot()
{
  auto header = (MultibootHeader*)s_KernelEndRounded;
  Memory::Copy((void*)s_KernelEndRounded, (void*)&s_Header,
               sizeof(MultibootHeader));
  s_KernelEndRounded += sizeof(MultibootHeader);

  header->Flags |= MULTIBOOT_FLAGS_MEM;

  SET_MB_MEMBER_STRING(header->CmdLine, L"");
  SET_MB_MEMBER_STRING(header->BootLoaderName, L"Kittnus Neo");

  auto graphicsMode = Graphics::GetMode();
  auto graphicsInfo = graphicsMode->Info;
  header->FramebufferAddr = graphicsMode->FrameBufferBase;
  header->FramebufferPitch = graphicsInfo->PixelsPerScanLine * 4;
  header->FramebufferWidth = graphicsInfo->HorizontalResolution;
  header->FramebufferHeight = graphicsInfo->VerticalResolution;
  header->FramebufferBpp = 32;

  RealignMemory();
}

void Kernel::CreateMemoryMap()
{
  auto mmap = (MultibootMemoryMap*)s_KernelEndRounded;

  Memory::Zero((void*)s_KernelEndRounded, 0x400);
  s_Header.MmapAddr = s_KernelEndRounded;

  UInt64 mmapSize, mapKey, descriptorSize;
  EFI_CHECK(Global::BootServices->GetMemoryMap(&mmapSize, NULL, &mapKey,
                                               &descriptorSize, NULL),
            L"Failed to get memory map size");

  auto memory = (EFI_MEMORY_DESCRIPTOR*)s_KernelEndRounded;
  s_KernelEntry += mmapSize;
  while ((UInt64)s_KernelEndRounded & 0x3FF) s_KernelEndRounded++;

  EFI_CHECK(Global::BootServices->GetMemoryMap(&mmapSize, memory, &mapKey,
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

  s_Header.MmapLength = (UInt32)((UInt64)mmap - s_Header.MmapAddr);

  s_Header.MemLower = 0x400;
  s_Header.MemUpper = upperMemory / 0x400;
}
