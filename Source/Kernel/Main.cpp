#include "GDTLoader.h"
#include "MemoryManager.h"
#include "UART.h"

extern "C" void KernelEntry()
{
  for (int i = 0; i < 300000000; i++) asm("nop"); // TODO: Remove

  asm("cli");

  for (int i = 0; i < 300000000; i++) asm("nop"); // TODO: Remove

  UART::Initialize();

  for (int i = 0; i < 300000000; i++) asm("nop"); // TODO: Remove

  UART::Print("Kernel entry point reached.");

  for (int i = 0; i < 300000000; i++) asm("nop"); // TODO: Remove

  // Start by setting up GDT
  GDTLoader::Load();
  MemoryManager::Initialize();

  // Now we prepare the memory

  asm("sti");
}
