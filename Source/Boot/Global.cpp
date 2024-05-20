#include "Global.h"

void Global::Initialize(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    Global::SystemTable = SystemTable;
    Global::ImageHandle = ImageHandle;

    Global::ConOut = SystemTable->ConOut;
    Global::ConIn = SystemTable->ConIn;
}
