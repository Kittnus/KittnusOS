#pragma once

#include "Types.h"

#define PACKED __attribute__((packed))

#define MULTIBOOT_MAGIC     0x1BADB002
#define MULTIBOOT_EAX_MAGIC 0x2BADB002

#define MULTIBOOT_FLAGS_MEM         0x1
#define MULTIBOOT_FLAGS_DEVICE      0x2
#define MULTIBOOT_FLAGS_CMDLINE     0x4
#define MULTIBOOT_FLAGS_MODS        0x8
#define MULTIBOOT_FLAGS_AOUT        0x10
#define MULTIBOOT_FLAGS_ELF         0x20
#define MULTIBOOT_FLAGS_MMAP        0x40
#define MULTIBOOT_FLAGS_DRIVE       0x80
#define MULTIBOOT_FLAGS_CONFIG      0x100
#define MULTIBOOT_FLAGS_LOADER_NAME 0x200
#define MULTIBOOT_FLAGS_APM         0x400
#define MULTIBOOT_FLAGS_VBE         0x800

struct MultibootHeader
{
  UInt32 Flags;

  UInt32 MemLower;
  UInt32 MemUpper;

  UInt32 BootDevice;

  UInt32 CmdLine;

  UInt32 ModsCount;
  UInt32 ModsAddr;

  UInt32 Num;
  UInt32 Size;
  UInt32 Addr;
  UInt32 Shndx;

  UInt32 MmapLength;
  UInt32 MmapAddr;

  UInt32 DrivesLength;
  UInt32 DrivesAddr;

  UInt32 ConfigTable;

  UInt32 BootLoaderName;

  UInt32 ApmTable;

  UInt32 VbeControlInfo;
  UInt32 VbeModeInfo;
  UInt32 VbeMode;
  UInt32 VbeInterfaceSeg;
  UInt32 VbeInterfaceOff;
  UInt32 VbeInterfaceLen;

  UInt32 FramebufferAddr;
  UInt32 FramebufferPitch;
  UInt32 FramebufferWidth;
  UInt32 FramebufferHeight;
  UInt8 FramebufferBpp;
  UInt8 FramebufferType;
} PACKED;

struct MultibootModule
{
  UInt32 ModStart;
  UInt32 ModEnd;
  UInt32 String;
  UInt32 Reserved;
} PACKED;

struct MultibootMemoryMap
{
  UInt32 Size;
  UInt64 BaseAddr;
  UInt64 Length;
  UInt32 Type;
} PACKED;
