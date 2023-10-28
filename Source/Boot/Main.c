#include "EFI/Libs.h"

EFI_STATUS EFIMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    InitializeEFI(imageHandle, systemTable);

    PrintLn(L"Hello, world!");

    SetColor(EFI_LIGHTGREEN);
    PrintLn(L"Press any key to exit...");
    ResetColor();

    WaitForKey();
    Shutdown();

    return EFI_SUCCESS;
}
