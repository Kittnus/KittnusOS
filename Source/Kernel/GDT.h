#pragma once

#include "Types.h"

#define PACKED __attribute__((packed))
#define ALIGN  __attribute__((aligned(0x1000)))

struct GDTDescriptor
{
  UInt16 Size;
  UInt64 Offset;
} PACKED;

struct GDTEntry
{
  UInt16 LimitLow;
  UInt16 BaseLow;
  UInt8 BaseMiddle;
  UInt8 Access;
  UInt8 Granularity;
  UInt8 BaseHigh;
} PACKED;

struct GDT
{
  GDTEntry Null; // 0x0

  GDTEntry Ring0Code; // 0x8
  GDTEntry Ring0Data; // 0x10

  GDTEntry Ring3Null; // 0x18
  GDTEntry Ring3Code; // 0x20
  GDTEntry Ring3Data; // 0x28

  GDTEntry TSSLow; // 0x30
  GDTEntry TSSHigh; // 0x38
} PACKED ALIGN;

extern GDT g_GDT;
