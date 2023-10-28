#include "EFI/Libs.h"

EFI_STATUS EFIMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    InitializeEFI(imageHandle, systemTable);
    PrintLn(L"Welcome to Kittnus Bootloader...");

    while (1)
    {
        CHAR16 *command = ReadLn();
        // TODO: tolower command using EFI_UNICODE_COLLATION_PROTOCOL

        if (StrCmp(command, L"help"))
        {
            PrintLn(L"Commands:");
            SetCursorColumn(2);
            PrintLn(L"shutdown - shuts down the computer.");
            PrintLn(L"restart - restarts the computer.");
            PrintLn(L"reboot - reboots the computer.");
        }
        else if (StrCmp(command, L"shutdown"))
            Shutdown();
        else if (StrCmp(command, L"restart"))
            Restart();
        else if (StrCmp(command, L"reboot"))
            Reboot();
    }

    EFI_PHYSICAL_ADDRESS physicalBuffer;
    UINT64 allocSize = (1 + 64) * 1024 * 1024;
    UINT64 pages = allocSize / 4096;
    EFI_CALL(SystemTable->BootServices->AllocatePages(
        AllocateAnyPages,
        EfiBootServicesData,
        pages,
        &physicalBuffer));

    EFI_PHYSICAL_ADDRESS physicalBufferKernelFile;
    UINT64 maxKernelFileSize = 4 * 1024 * 1024;
    UINT64 kernelFilePages = maxKernelFileSize / 4096;
    EFI_CALL(SystemTable->BootServices->AllocatePages(
        AllocateAnyPages,
        EfiBootServicesData,
        kernelFilePages,
        &physicalBufferKernelFile));

    UINT64 stackStart = physicalBuffer;
    UINT64 stackSize = 1024 * 1024;
    UINT64 kernelStart = stackStart + stackSize;
    UINT64 kernelMaxSize = allocSize - stackSize;

    return EFI_SUCCESS;
}
