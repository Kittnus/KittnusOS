#include "Port.h"

void Port::OutPortB(UInt16 port, UInt8 data)
{
  asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}