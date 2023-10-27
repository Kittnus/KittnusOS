#include "UEFI.h"

static void* uefi_image_handle;
static EFI_SYSTEM_TABLE uefi_system_table;

EFI_STATUS efi_main(void* image_handle, EFI_SYSTEM_TABLE system_table)
{
    uefi_image_handle = image_handle;
    uefi_system_table = system_table;
}
