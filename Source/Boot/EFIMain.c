#include "UEFI.h"

EFI_STATUS EfiMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    systemTable->ConOut->Reset(systemTable->ConOut, 1);
    systemTable->ConOut->OutputString(systemTable->ConOut, L"Hello, World!\n\r");

    while (1)
    {
        // Do nothing
    };

    return EFI_SUCCESS;
}
