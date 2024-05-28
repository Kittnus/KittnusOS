#pragma once

#include "Types.h"

class MemoryManager
{
public:
  static void Initialize();

private:
  static void InitializePaging();
  static void InitializeHeap();
};
