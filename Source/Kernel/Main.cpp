#include "GDTLoader.h"
#include "MemoryManager.h"
#include "UART.h"

extern "C" void KernelEntry()
{
  asm("cli");

  UART::Initialize();
  return;

  UART::Print("Kernel entry point reached.");

  // Start by setting up GDT
  GDTLoader::Load();
  MemoryManager::Initialize();

  // Now we prepare the memory

  asm("sti");

  while (true);
}
