#include "EFI.h"

static EFI_BOOT_SERVICES *BootServices;
static EFI_RUNTIME_SERVICES *RuntimeServices;

static EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
static EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;

void InitializeEFI(EFI_SYSTEM_TABLE *systemTable)
{
    BootServices = systemTable->BootServices;
    RuntimeServices = systemTable->RuntimeServices;

    ConOut = systemTable->ConOut;
    ConOut->Reset(ConOut, TRUE);

    ConIn = systemTable->ConIn;
    ConIn->Reset(ConIn, TRUE);
}

EFI_STATUS EFIMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    InitializeEFI(systemTable);

    ConOut->OutputString(ConOut, L"Hello world!\n\r");

    ConOut->SetAttribute(ConOut, EFI_LIGHTGREEN);
    ConOut->OutputString(ConOut, L"Press any key to exit...\n\r");

    EFI_STATUS status;
    BootServices->WaitForEvent(1, &ConIn->WaitForKey, &status);

    return EFI_SUCCESS;
}
