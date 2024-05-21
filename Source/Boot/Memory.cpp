#include "Memory.h"

void Memory::Copy(void* destination, const void* source, UInt64 count)
{
  asm volatile("rep movsb" : : "D"(destination), "S"(source), "c"(count) :);
}

void Memory::Fill(void* destination, int value, UInt64 count)
{
  asm volatile("rep stosb" : : "D"(destination), "a"(value), "c"(count) :);
}

void Memory::Zero(void* destination, UInt64 count)
{
  Fill(destination, 0, count);
}
