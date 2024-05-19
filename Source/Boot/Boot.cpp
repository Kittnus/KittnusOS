#define EFI_PLATFORM EFI_ARCH_X64
#include <efi.h>

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello, UEFI!\r\n");
    return EFI_SUCCESS;
}
