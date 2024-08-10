#include "SerialPort.h"

#include "HAL/IO.h"

void SerialPort::Initialize() {}

void SerialPort::Print(const char* data)
{
  auto c = (char*)data;
  while (*c != 0) WriteByte((UInt8)*c++);
}

void SerialPort::PrintLn(const char* data)
{
  Print(data);
  Print("\r\n");
}

void SerialPort::WriteByte(UInt8 byte) { IO::Write8(COM1, byte); }
