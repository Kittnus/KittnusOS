#pragma once

#include "Types.h"

class Memory
{
public:
  static void Copy(void* destination, const void* source, UInt64 count);
  static void Fill(void* destination, int value, UInt64 count);
  static void Zero(void* destination, UInt64 count);
};
