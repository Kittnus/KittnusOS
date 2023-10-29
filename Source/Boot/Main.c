#include "EFI/Libs.h"

typedef struct
{
    CHAR16 *commandName;
    CHAR16 *commandDescription;
    void *handler;
} CommandInfo;

void ShowHelp();

void LoadKernel()
{
    EFI_PHYSICAL_ADDRESS physicalBuffer;
    UINT64 allocSize = (1 + 64) * 1024 * 1024;
    UINT64 pages = allocSize / 4096;
    EFI_CALL(BootServices->AllocatePages(
        AllocateAnyPages,
        EfiBootServicesData,
        pages,
        &physicalBuffer));

    EFI_PHYSICAL_ADDRESS physicalBufferKernelFile;
    UINT64 kernelFileSizeMax = 4 * 1024 * 1024;
    UINT64 kernelFilePages = kernelFileSizeMax / 4096;
    EFI_CALL(BootServices->AllocatePages(
        AllocateAnyPages,
        EfiBootServicesData,
        kernelFilePages,
        &physicalBufferKernelFile));

    UINT64 stackStart = physicalBuffer;
    UINT64 stackSize = 1024 * 1024;
    UINT64 kernelStart = stackStart + stackSize;
    UINT64 kernelMaxSize = allocSize - stackSize;

    EFI_FILE_PROTOCOL *file;
    EFI_CALL(FileSystem->OpenVolume(FileSystem, &file));

    char *buffer = (char *)physicalBufferKernelFile;
    EFI_CALL(file->GetInfo(file, &FileSystemInfoId, &kernelFileSizeMax, buffer));

    EFI_FILE_SYSTEM_INFO *fileSystemInfo = (EFI_FILE_SYSTEM_INFO *)buffer;
    Print(L"Volume label: ");
    PrintLn(fileSystemInfo->VolumeLabel);

    Print(L"Volume size: ");
    PrintLn(fileSystemInfo->VolumeSize);

    Print(L"Free space: ");
    PrintLn(fileSystemInfo->FreeSpace);

    Print(L"Block size: ");
    PrintLn(fileSystemInfo->BlockSize);

    EFI_FILE_PROTOCOL *fileHandle;
    EFI_CALL(file->Open(file, &fileHandle, L"Kernel", EFI_FILE_MODE_READ, 0));

    EFI_CALL(fileHandle->GetInfo(fileHandle, &FileInfoId, &kernelMaxSize, buffer));

    EFI_FILE_INFO *fileInfo = buffer;
    Print(L"File name: ");
    PrintLn(fileInfo->FileName);

    Print(L"File size: ");
    PrintLn(fileInfo->FileSize);

    if (fileInfo->FileSize > kernelFileSizeMax)
    {
        PrintLn(L"Kernel file is too big");
        return EFI_LOAD_ERROR;
    }

    UINTN readSize = fileInfo->FileSize;
    EFI_CALL(fileHandle->Read(fileHandle, &readSize, buffer));

    // EFI_CALL(fileHandle->Close(fileHandle));
}

CommandInfo commands[] = {
    {L"Help", L"Show available commands", ShowHelp},
    {L"LoadKernel", L"Load the kernel", LoadKernel},
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

    return EFI_SUCCESS;
}
