#include "EFI/Libs.h"

EFI_STATUS EFIMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    InitializeLibs(imageHandle, systemTable);

    PrintLn(L"Hello, world!");

    SetColor(EFI_LIGHTGREEN);
    PrintLn(L"Press any key to exit...");

    EFI_STATUS status;
    BootServices->WaitForEvent(1, &ConIn->WaitForKey, &status);

    return EFI_SUCCESS;
}
