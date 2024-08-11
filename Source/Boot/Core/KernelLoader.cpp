#include "KernelLoader.h"

#include "Global.h"
#include "Services/FileSystem.h"
#include "Services/Memory.h"
#include "UI/Graphics.h"
#include "Utilities/Macros.h"
#include "Utilities/String.h"

void KernelLoader::Load()
{
  LoadFile(L"Kernel.elf");
  LoadElf();
}

[[noreturn]] void KernelLoader::Execute()
{
  CHECK(s_EntryAddress, "Kernel entry point is null.");

  // TODO: Remove debug code
  Graphics::Print("Kernel entry point: ");
  Graphics::PrintHexLn(s_EntryAddress);
  // TODO: End debug code

  typedef void (*KernelEntry)();
  auto kernelMain = (KernelEntry)s_EntryAddress;

  kernelMain();
  while (true);
}

void KernelLoader::LoadFile(const wchar_t* name)
{
  auto file = FileSystem::OpenFile(name, EFI_FILE_MODE_READ);

  AllocateMemory();

  UInt64 fileSize = 0;
  file->SetPosition(file, 0xFFFFFFFFFFFFFFFFULL);
  file->GetPosition(file, &fileSize);
  file->SetPosition(file, 0);

  EFI_CHECK(file->Read(file, &fileSize, (void*)KERNEL_LOAD_ADDRESS),
            "Failed to read kernel file");

  file->Close(file);
}

void KernelLoader::LoadElf()
{
  auto header = (ElfHeader*)KERNEL_LOAD_ADDRESS;

  CHECK(header->e_Ident[0] == 0x7F || header->e_Ident[1] == 'E'
            || header->e_Ident[2] == 'L' || header->e_Ident[3] == 'F',
        "Invalid ELF header.");

  CHECK(header->e_Ident[4] == ELFCLASS64,
        "Unsupported ELF format."); // TODO: Support 32-bit

  UInt64 imageBase = 0;
  for (UInt64 i = 0; i < header->e_PhdrEntrySize * header->e_PhdrCount;
       i += header->e_PhdrEntrySize)
  {
    auto programHeader =
        (ElfProgramHeader*)(KERNEL_LOAD_ADDRESS + header->e_PhdrOffset + i);
    if (programHeader->p_Type != PT_LOAD) continue;

    if (!imageBase) imageBase = programHeader->p_VAddr;

    auto segment = (UInt8*)(programHeader->p_VAddr);
    auto fileData = (UInt8*)(KERNEL_LOAD_ADDRESS + programHeader->p_Offset);

    Memory::Copy(segment, fileData, programHeader->p_Filesz);

    auto remainingSize = programHeader->p_Filesz;
    while (remainingSize < programHeader->p_Memsz)
      *(segment + remainingSize++) = 0;
  }

  auto entryOffset = header->e_Entry - imageBase;
  s_EntryAddress = KERNEL_LOAD_ADDRESS + entryOffset;
}

// TODO: Implement Kernel Address Space Layout Randomization (KASLR)
void KernelLoader::AllocateMemory()
{
  EFI_PHYSICAL_ADDRESS kernelStart = KERNEL_LOAD_ADDRESS;
  EFI_CHECK(Global::BootServices->AllocatePages(AllocateAddress, EfiLoaderData,
                                                0x2000, &kernelStart),
            "Failed to allocate memory for kernel");
}
