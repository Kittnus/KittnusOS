#include "Kernel.h"

#include "FileSystem.h"
#include "Global.h"
#include "Graphics.h"
#include "Memory.h"
#include "String.h"

void Kernel::Initialize()
{
  s_KernelEntry = LoadFile(L"Kernel.bin");

  // Create the memory map
}

void Kernel::Execute()
{
  // TODO: Remove debug code
  Graphics::PrintLn("Executing kernel entry point...");
  // TODO: End debug code

  typedef void (*KernelEntry)();
  auto kernelEntry = (KernelEntry)s_KernelEntry;

  kernelEntry();

  while (true);
}

UInt64 Kernel::LoadFile(const wchar_t* name)
{
  auto file = FileSystem::OpenFile(name, EFI_FILE_MODE_READ);

  UInt64 fileSize = 0;
  file->SetPosition(file, 0xFFFFFFFFFFFFFFFFULL);
  file->GetPosition(file, &fileSize);
  file->SetPosition(file, 0);

  void* fileBuffer = NULL;
  Memory::Allocate(fileSize, &fileBuffer);
  file->Read(file, &fileSize, fileBuffer);

  return (UInt64)fileBuffer;
}
