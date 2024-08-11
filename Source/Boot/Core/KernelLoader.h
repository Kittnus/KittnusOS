#pragma once

#include "Elf.h"
#include "Types.h"

#define KERNEL_LOAD_ADDRESS 0x4000000

class KernelLoader
{
private:
  static inline UInt64 s_EntryAddress;

public:
  static void Load();
  [[noreturn]] static void Execute();

private:
  static void LoadFile(const wchar_t* name);
  static void LoadElf();

  static void AllocateMemory();
};
