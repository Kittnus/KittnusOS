#include "IO.h"

UInt8 IO::Read8(UInt16 port)
{
  UInt8 data;
  asm volatile("inb %w1, %b0" : "=a"(data) : "Nd"(port));
  return data;
}

UInt16 IO::Read16(UInt16 port)
{
  UInt16 data;
  asm volatile("inw %w1, %w0" : "=a"(data) : "Nd"(port));
  return data;
}

UInt32 IO::Read32(UInt16 port)
{
  UInt32 data;
  asm volatile("inl %w1, %0" : "=a"(data) : "Nd"(port));
  return data;
}

void IO::Write8(UInt16 port, UInt8 data)
{
  asm volatile("outb %b0, %w1" : : "a"(data), "Nd"(port));
}

void IO::Write16(UInt16 port, UInt16 data)
{
  asm volatile("outw %w0, %w1" : : "a"(data), "Nd"(port));
}

void IO::Write32(UInt16 port, UInt32 data)
{
  asm volatile("outl %0, %w1" : : "a"(data), "Nd"(port));
}
