#include "EFI/Libs.h"

EFI_STATUS EFIMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    InitializeEFI(imageHandle, systemTable);

    PrintLn(L"Welcome to Kittnus Bootloader...");

    while (1)
    {
        CHAR16 *command = ReadLn();

        if (command == L"help")
        {
            PrintLn(L"Commands:");
            PrintLn(L"\tshutdown - shuts down the computer.");
            PrintLn(L"\trestart - restarts the computer.");
            PrintLn("\treboot - reboots the computer.");
        }
        else if (command == L"shutdown")
        {
            Shutdown();
        }
        else if (command == L"restart")
        {
            Restart();
        }
        else if (command == L"reboot")
        {
            Reboot();
        }
    }

    return EFI_SUCCESS;
}
