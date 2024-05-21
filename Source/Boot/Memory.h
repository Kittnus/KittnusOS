#pragma once

class Memory
{
public:
  static void Copy(void* destination, const void* source, UINTN count);
  static void Fill(void* destination, int value, UINTN count);
  static void Zero(void* destination, UINTN count);
};
