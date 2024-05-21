#include "Global.h"

void Global::Initialize(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
  MultibootHeader = {
    MULTIBOOT_FLAGS_MEM | MULTIBOOT_FLAGS_CMDLINE | MULTIBOOT_FLAGS_MMAP | MULTIBOOT_FLAGS_LOADER_NAME, // Flags

    0x100000, // MemLower
    0x640000, // MemUpper

    0, // BootDevice

    0, // CmdLine

    0, // ModsCount
    0, // ModsAddr

    0, // Num
    0, // Size
    0, // Addr
    0, // Shndx

    0, // MmapLength
    0, // MmapAddr

    0, // DrivesLength
    0, // DrivesAddr

    0, // ConfigTable

    0, // BootLoaderName

    0, // ApmTable

    0, // VbeControlInfo
    0, // VbeModeInfo
    0, // VbeMode
    0, // VbeInterfaceSeg
    0, // VbeInterfaceOff
    0, // VbeInterfaceLen

    0, // FramebufferAddr
    0, // FramebufferPitch
    0, // FramebufferWidth
    0, // FramebufferHeight
    0, // FramebufferBpp
    0, // FramebufferType
  };

  KernelEntryAddress = 0;
  KernelRoundedAddress = 0;

  SystemTable = systemTable;
  ImageHandle = imageHandle;

  BootServices = SystemTable->BootServices;

  ConOut = SystemTable->ConOut;
  ConIn = SystemTable->ConIn;

  GraphicsOutput = nullptr;
}
