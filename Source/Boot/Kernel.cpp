#include "Kernel.h"

#include "FileSystem.h"
#include "Global.h"
#include "Graphics.h"
#include "Macros.h"
#include "Memory.h"
#include "String.h"

void Kernel::Load()
{
  LoadFile(L"Kernel.elf");
  LoadElf();
}

void Kernel::Execute()
{
  // TODO: Remove debug code
  Graphics::PrintLn("Executing kernel entry point...");
  // TODO: End debug code

  CHECK(s_KernelEntry, "Kernel entry point is null.");

  typedef void (*KernelEntry)();
  auto kernelEntry = (KernelEntry)s_KernelEntry;
  kernelEntry();

  // TODO: Remove debug code
  Graphics::PrintLn("Kernel entry point returned.");
  // TODO: End debug code

  while (true);
}

void Kernel::LoadFile(const wchar_t* name)
{
  auto file = FileSystem::OpenFile(name, EFI_FILE_MODE_READ);

  AllocateMemory();

  UInt64 fileSize = 0;
  file->SetPosition(file, 0xFFFFFFFFFFFFFFFFULL);
  file->GetPosition(file, &fileSize);
  file->SetPosition(file, 0);

  EFI_CHECK(file->Read(file, &fileSize, (void*)c_KernelStart),
            "Failed to read kernel file.");

  file->Close(file);
}

void Kernel::LoadElf()
{
  auto header = (ElfHeader*)c_KernelStart;

  CHECK(header->e_Ident[0] == 0x7F || header->e_Ident[1] == 'E'
            || header->e_Ident[2] == 'L' || header->e_Ident[3] == 'F',
        "Invalid ELF header.");

  CHECK(header->e_Ident[4] == ELFCLASS64,
        "Unsupported ELF format."); // TODO: Support 32-bit

  s_KernelEntry = header->e_Entry;
  for (UInt64 i = 0; i < header->e_PhdrEntrySize * header->e_PhdrCount;
       i += header->e_PhdrEntrySize)
  {
    auto programHeader =
        (ElfProgramHeader*)(c_KernelStart + header->e_PhdrOffset + i);
    if (programHeader->p_Type != PT_LOAD) continue;

    auto segment = (UInt8*)(programHeader->p_VAddr);
    Memory::Copy(segment, (void*)(c_KernelStart + programHeader->p_Offset),
                 programHeader->p_Filesz);

    auto remainingSize = programHeader->p_Filesz;
    while (remainingSize < programHeader->p_Memsz)
      *(UInt8*)(programHeader->p_VAddr + remainingSize++) = 0;
  }
}

void Kernel::AllocateMemory()
{
  EFI_PHYSICAL_ADDRESS kernelStart = c_KernelStart;
  EFI_CHECK(Global::BootServices->AllocatePages(AllocateAddress, EfiLoaderData,
                                                0x2000, &kernelStart),
            "Failed to allocate memory for kernel.");
}
