#include "Global.h"

void Global::Initialize(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
  SystemTable = systemTable;
  ImageHandle = imageHandle;

  BootServices = SystemTable->BootServices;

  ConOut = SystemTable->ConOut;
  ConIn = SystemTable->ConIn;
}
