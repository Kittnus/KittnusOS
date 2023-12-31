#pragma once

#define VERSION_MAJOR (__DATE__[7] - '0') * 1000 + (__DATE__[8] - '0') * 100 + (__DATE__[9] - '0')
#define VERSION_MINOR (__DATE__[0] - 'A' + 1) * 100 + (__DATE__[1] - 'A' + 1)
#define VERSION_PATCH (__DATE__[4] - '0') * 1000 + (__TIME__[0] - '0') * 100 + (__TIME__[1] - '0')

BOOLEAN IsInShell = FALSE;

EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;

EFI_BOOT_SERVICES *BootServices;
EFI_RUNTIME_SERVICES *RuntimeServices;

EFI_GUID UnicodeInterfaceGuid = EFI_UNICODE_COLLATION_PROTOCOL2_GUID;
EFI_UNICODE_COLLATION_PROTOCOL *UnicodeInterface;

EFI_GUID SimpleFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;

EFI_GUID FileInfoId = EFI_FILE_INFO_ID;
EFI_GUID FileSystemInfoId = EFI_FILE_SYSTEM_INFO_ID;
