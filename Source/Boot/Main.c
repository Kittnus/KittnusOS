#include "EFI/Types.h"
#include "EFI/Errors.h"
#include "EFI/API.h"
#include "EFI/Libs.h"
#include "Config.h"
#include "Shell.h"

EFI_STATUS EFIMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    InitializeLibs(imageHandle, systemTable);

    if (ShouldStartShell)
        OpenShell();

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

    // UINT64 stackStart = physicalBuffer;
    UINT64 stackSize = 1024 * 1024;
    // UINT64 kernelStart = stackStart + stackSize;
    UINT64 kernelMaxSize = allocSize - stackSize;

    EFI_FILE_PROTOCOL *file;
    EFI_CALL(FileSystem->OpenVolume(FileSystem, &file));

    char *buffer = (char *)physicalBufferKernelFile;
    EFI_CALL(file->GetInfo(file, &FileSystemInfoId, &kernelFileSizeMax, buffer));

    EFI_FILE_SYSTEM_INFO *fileSystemInfo = (EFI_FILE_SYSTEM_INFO *)buffer;
    Print(L"Volume label: ");
    PrintLn(fileSystemInfo->VolumeLabel);

    Print(L"Volume size: ");
    PrintIntLn(fileSystemInfo->VolumeSize);

    Print(L"Free space: ");
    PrintIntLn(fileSystemInfo->FreeSpace);

    Print(L"Block size: ");
    PrintIntLn(fileSystemInfo->BlockSize);

    EFI_FILE_PROTOCOL *fileHandle;
    EFI_CALL(file->Open(file, &fileHandle, L"Kernel", EFI_FILE_MODE_READ, 0));

    EFI_CALL(fileHandle->GetInfo(fileHandle, &FileInfoId, &kernelMaxSize, buffer));

    EFI_FILE_INFO *fileInfo = (EFI_FILE_INFO *)buffer;
    Print(L"File name: ");
    PrintLn(fileInfo->FileName);

    Print(L"File size: ");
    PrintIntLn(fileInfo->FileSize);

    if (fileInfo->FileSize > kernelFileSizeMax)
    {
        PrintLn(L"Kernel file is too big");
        return EFI_LOAD_ERROR;
    }

    UINTN readSize = fileInfo->FileSize;
    EFI_CALL(fileHandle->Read(fileHandle, &readSize, buffer));

    // EFI_CALL(fileHandle->Close(fileHandle));

    return EFI_SUCCESS;
}
