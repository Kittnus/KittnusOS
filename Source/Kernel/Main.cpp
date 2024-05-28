#include "GDTLoader.h"
#include "UART.h"
#include "MemoryManager.h"

extern "C" void KernelEntry()
{
  return;
  asm("cli");

  UART::Initialize();

  UART::Print("Kernel entry point reached.");

  return;

  // Start by setting up GDT
  GDTLoader::Load();
  MemoryManager::Initialize();

  // Now we prepare the memory

  asm("sti");
}
