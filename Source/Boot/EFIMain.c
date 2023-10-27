#include "EFI.h"

EFI_STATUS EfiMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    systemTable->ConOut->Reset(systemTable->ConOut, 1);
    systemTable->ConOut->OutputString(systemTable->ConOut, L"Hello world!\n\r");

    systemTable->ConOut->SetAttribute(systemTable->ConOut, EFI_LIGHTGREEN);
    systemTable->ConOut->OutputString(systemTable->ConOut, L"Press any key to exit...\n\r");

    systemTable->ConIn->Reset(systemTable->ConIn, 1);
    EFI_STATUS status;
    systemTable->BootServices->WaitForEvent(1, &systemTable->ConIn->WaitForKey, &status);

    return EFI_SUCCESS;
}
