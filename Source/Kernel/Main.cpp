#include "UART.h"

extern "C" int KernelMain()
{
  asm("cli");

  UART::Initialize();
  return 69;
  UART::Print("Kernel entry point reached.");

  asm("sti");

  while (true);
}
