#pragma once

#include "Types.h"
#include "EfiMacros.h"

class Kernel
{
private:
  static inline UInt64 s_KernelEntry;
  static inline UInt64 s_KernelEndRounded;

public:
  static void Initialize();
  static void Execute();

private:
  static void Load();

  static UInt64 FindEntryPoint(EFI_FILE_PROTOCOL* file);

  static void RealignMemory();
};
