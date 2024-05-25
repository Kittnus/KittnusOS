#pragma once

#include "Types.h"

class Kernel
{
private:
  static inline UInt64 s_KernelEntry;

public:
  static void Initialize();
  static void Execute();

private:
  static UInt64 LoadFile(const wchar_t* name);
};
