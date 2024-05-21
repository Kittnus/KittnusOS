#include "Memory.h"

#include "Global.h"

void Memory::Copy(void* destination, const void* source, UINTN count)
{
  asm volatile("rep movsb" : : "D"(destination), "S"(source), "c"(count) :);
}

void Memory::Fill(void* destination, int value, UINTN count)
{
  asm volatile("rep stosb" : : "D"(destination), "a"(value), "c"(count) :);
}

void Memory::Zero(void* destination, UINTN count)
{
  Fill(destination, 0, count);
}
