#include "UART.h"

extern "C" int KernelMain()
{
  asm("cli");

  UART::Initialize();
  UART::Print("Kernel entry point reached.");

  asm("sti");
}
