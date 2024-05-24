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

void Kernel::Initialize()
{
  Load();

  // Create the memory map
}

void Kernel::Execute()
{
  typedef void (*KernelEntry)(UInt32);
  ((KernelEntry)s_KernelEntry)(3);

  Graphics::PrintLn(L"Kernel returned. Halting system...");
  while (true);
}

void Kernel::Load()
{
  // constexpr UInt64 KERNEL_LOAD_ADDRESS = 0x4000000; // 64 MB offset

  auto kernelFile = FileSystem::OpenFile(L"Kernel.bin", EFI_FILE_MODE_READ);
  s_KernelEntry = FindEntryPoint(kernelFile);
}

// We call this everytime we allocate memory to make sure it's aligned to 4 KiB
void Kernel::RealignMemory()
{
  s_KernelEndRounded = (s_KernelEndRounded & ~0xFFF)
                       + ((s_KernelEndRounded & 0xFFF) ? 0x1000 : 0);
}

UInt64 Kernel::FindEntryPoint(EFI_FILE_PROTOCOL* file)
{
  UInt64 fileSize = 0;
  file->SetPosition(file, 0xFFFFFFFFFFFFFFFFULL);
  file->GetPosition(file, &fileSize);
  file->SetPosition(file, 0);

  void* fileBuffer = NULL;
  Memory::Allocate(fileSize, &fileBuffer);
  file->Read(file, &fileSize, fileBuffer);

  auto entryPoint = (UInt8*)fileBuffer;
  (void)entryPoint;

  return 0;
}
