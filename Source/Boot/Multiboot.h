#pragma once

#define PACKED __attribute__((packed))

#define MULTIBOOT_MAGIC             0x1BADB002
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

struct PACKED MultibootHeader
{
  UINT32 Flags;

  UINT32 MemLower;
  UINT32 MemUpper;

  UINT32 BootDevice;

  UINT32 CmdLine;

  UINT32 ModsCount;
  UINT32 ModsAddr;

  UINT32 Num;
  UINT32 Size;
  UINT32 Addr;
  UINT32 Shndx;

  UINT32 MmapLength;
  UINT32 MmapAddr;

  UINT32 DrivesLength;
  UINT32 DrivesAddr;

  UINT32 ConfigTable;

  UINT32 BootLoaderName;

  UINT32 ApmTable;

  UINT32 VbeControlInfo;
  UINT32 VbeModeInfo;
  UINT32 VbeMode;
  UINT32 VbeInterfaceSeg;
  UINT32 VbeInterfaceOff;
  UINT32 VbeInterfaceLen;

  UINT32 FramebufferAddr;
  UINT32 FramebufferPitch;
  UINT32 FramebufferWidth;
  UINT32 FramebufferHeight;
  UINT8 FramebufferBpp;
  UINT8 FramebufferType;
};

struct PACKED MultibootModule
{
  UINT32 ModStart;
  UINT32 ModEnd;
  UINT32 String;
  UINT32 Reserved;
};

struct PACKED MultibootMemoryMap
{
  UINT32 Size;
  UINT64 BaseAddr;
  UINT64 Length;
  UINT32 Type;
};
