#include "Console.h"
#include "Types.h"

extern "C" void KernelEntry()
{
  Console::PrintLn("Hello, KittnusOS!");

  while (true);
}
