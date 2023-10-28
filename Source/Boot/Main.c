#include "EFI/Libs.h"

typedef struct
{
    CHAR16 *commandName;
    CHAR16 *commandDescription;
    void *handler;
} CommandInfo;

void ShowHelp();

CommandInfo commands[] = {
    {L"Help", L"Show available commands", ShowHelp},
    {L"Shutdown", L"Shut down the machine", Shutdown},
    {L"Restart", L"Restart the machine", Restart},
    {L"Reboot", L"Reboot the machine", Reboot}};

void ShowHelp()
{
    PrintLn(L"Commands:");
    for (UINTN i = 0; i < sizeof(commands) / sizeof(CommandInfo); i++)
    {
        SetCursorColumn(1);
        Print(commands[i].commandName);
        Print(L" - ");
        PrintLn(commands[i].commandDescription);
    }
}

void ExecuteCommand(CHAR16 *command)
{
    for (UINTN i = 0; i < sizeof(commands) / sizeof(CommandInfo); i++)
        if (StrCmp(command, commands[i].commandName) == 0)
        {
            ((void (*)())commands[i].handler)();
            return;
        }

    Print(L"Unknown command: ");
    PrintLn(command);
}

EFI_STATUS EFIMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    InitializeEFI(imageHandle, systemTable);
    PrintLn(L"Welcome to Kittnus bootloader...");

    while (1)
    {
        CHAR16 *command = ReadLn();
        ExecuteCommand(command);
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

    /*
    UINT64 stackStart = physicalBuffer;
    UINT64 stackSize = 1024 * 1024;
    UINT64 kernelStart = stackStart + stackSize;
    UINT64 kernelMaxSize = allocSize - stackSize;
    */

    return EFI_SUCCESS;
}
