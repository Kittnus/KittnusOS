#pragma once

#include "Elf.h"
#include "Multiboot.h"
#include "Types.h"

class Kernel
{
private:
  static inline UInt64 s_KernelEntry;
  static inline UInt64 s_KernelEndRounded;

  static inline MultibootHeader s_Header;

public:
  static void FindACPI();

  static void Initialize();
  static void Execute();

private:
  static void Load();
  static void LoadElf(Elf32Header* elfHeader);

  static void InitializeMultiboot();
  static void SetupMultiboot();

  static void CreateMemoryMap();

  static void RealignMemory();
};
