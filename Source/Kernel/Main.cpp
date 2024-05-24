#include "Types.h"

extern "C" void KernelEntry(UInt32 funny)
{
  while (funny > 0)
  {
    funny--;
    for (int i = 0; i < 100000000; i++);
  }
}
