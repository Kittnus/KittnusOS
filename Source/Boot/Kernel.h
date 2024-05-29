#pragma once

#include "Elf.h"
#include "Types.h"

class Kernel
{
private:
  static constexpr UInt64 c_KernelStart = 0x4000000; // 64 MB
  static inline UInt64 s_KernelEntry;

public:
  static void Load();
  static void Execute();

private:
  static void LoadFile(const wchar_t* name);
  static void LoadElf();

  static void AllocateMemory();
};
