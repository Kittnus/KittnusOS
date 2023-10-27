#include "UEFI.h"

EFI_STATUS EfiMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    systemTable->ConOut->OutputString(systemTable->ConOut, L"Hello, World!\n\r");
    
    return EFI_SUCCESS;
}
