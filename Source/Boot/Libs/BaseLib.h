#pragma once

void Print(CHAR16 *string);
void PrintLn(CHAR16 *string);
void PrintInt(UINT64 number);
void SetColor(UINTN color);
void ResetColor();

#define WIDEN(x) L##x

#define EFI_CALL(function)                          \
    do                                              \
    {                                               \
        EFI_STATUS status = function;               \
        if (EFI_ERROR(status))                      \
        {                                           \
            SetColor(EFI_RED);                      \
            Print(L"Error occurred in function: "); \
            PrintLn(WIDEN(#function));              \
            Print(L"Error message: ");              \
            Print(GetErrorString(status));          \
            Print(L" (");                           \
            PrintInt(status - ERRBASE);             \
            PrintLn(L")");                          \
            ResetColor();                           \
        }                                           \
    } while (0)

void LocateProtocol(EFI_GUID *protocol, void **interface)
{
    EFI_CALL(BootServices->LocateProtocol(protocol, 0, interface));
}

void InitializeLibs(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    ImageHandle = imageHandle;
    SystemTable = systemTable;

    ConOut = systemTable->ConOut;
    EFI_CALL(ConOut->Reset(ConOut, TRUE));

    ConIn = systemTable->ConIn;
    EFI_CALL(ConIn->Reset(ConIn, TRUE));

    BootServices = systemTable->BootServices;
    RuntimeServices = systemTable->RuntimeServices;

    LocateProtocol(&UnicodeInterfaceGuid, (void **)&UnicodeInterface);

    LocateProtocol(&SimpleFileSystemProtocolGuid, (void **)&Volume);
    EFI_CALL(Volume->OpenVolume(Volume, &RootFS));
}
