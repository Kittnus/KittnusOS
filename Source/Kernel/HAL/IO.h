#pragma once

#include "Types.h"

// TODO: Implement RDMSR
class IO
{
public:
  static UInt8 Read8(UInt16 port);
  static UInt16 Read16(UInt16 port);
  static UInt32 Read32(UInt16 port);

  static void Write8(UInt16 port, UInt8 data);
  static void Write16(UInt16 port, UInt16 data);
  static void Write32(UInt16 port, UInt32 data);
};
