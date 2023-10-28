#pragma once

#include "API.h"

EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;

EFI_BOOT_SERVICES *BootServices;
EFI_RUNTIME_SERVICES *RuntimeServices;

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;

void Print(CHAR16 *string)
{
    ConOut->OutputString(ConOut, string);
}

void PrintLn(CHAR16 *string)
{
    ConOut->OutputString(ConOut, string);
    ConOut->OutputString(ConOut, L"\r\n");
}

CHAR16 *UINT64ToString(UINT64 value)
{
    CHAR16 buffer[21];
    CHAR16 *result = buffer + 20;

    *result = L'\0';

    if (value == 0)
        *result-- = L'0';
    else
        while (value > 0)
        {
            *result-- = L'0' + (CHAR16)(value % 10);
            value /= 10;
        }

    return result;
}

#define EFI_CALL(function)              \
    do                                  \
    {                                   \
        EFI_STATUS status = function;   \
        if (EFI_ERROR(status))          \
        {                               \
            Print(L"Error: ");          \
            Print((CHAR16 *)#function); \
            Print(L" returned ");       \
            Print(UINT64ToString(status));              \
        }                               \
    } while (0)

void InitializeLibs(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    ImageHandle = imageHandle;
    SystemTable = systemTable;

    BootServices = systemTable->BootServices;
    RuntimeServices = systemTable->RuntimeServices;

    ConOut = systemTable->ConOut;
    ConOut->Reset(ConOut, TRUE);

    EFI_CALL(ConOut->Reset(ConOut, TRUE));
    EFI_CALL(ConIn->Reset(ConIn, TRUE));
}

void SetColor(UINTN color)
{
    EFI_CALL(ConOut->SetAttribute(ConOut, color));
}
