#include "UART.h"

#include "Memory.h"
#include "Port.h"

void UART::Initialize()
{
  Port::OutPortB(LINE_CONTROL_PORT, 0b00000011);
  PrintLn("UART initialized.");
}

void UART::Print(const char* data)
{
  auto c = (char*)data;
  while (*c != 0) WriteByte((UInt8)*c++);
}

void UART::PrintLn(const char* data)
{
  Print(data);
  Print("\r\n");
}

void UART::WriteByte(UInt8 byte) { Port::OutPortB(COM, byte); }
