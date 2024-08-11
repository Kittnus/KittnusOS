#include "Drivers/SerialPort.h"

extern "C" void KernelMain()
{
  asm("cli");

  SerialPort::Initialize();
  SerialPort::Print("Kernel entry point reached.");

  asm("sti");
}
