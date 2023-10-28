#include "EFI/Libs.h"

EFI_STATUS EFIMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    InitializeEFI(imageHandle, systemTable);

    PrintLn(L"Hello, world!");

    SetColor(EFI_LIGHTGREEN);
    PrintLn(L"Press any key to exit...");
    ResetColor();

    WaitForKey();

    PrintLn(L"Shutting down...");
    Shutdown();

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
