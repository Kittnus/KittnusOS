#pragma once

#include "API.h"

EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;

EFI_BOOT_SERVICES *BootServices;
EFI_RUNTIME_SERVICES *RuntimeServices;

void Print(CHAR16 *string)
{
    ConOut->OutputString(ConOut, string);
}

void PrintLn(CHAR16 *string)
{
    ConOut->OutputString(ConOut, string);
    ConOut->OutputString(ConOut, L"\r\n");
}

void SetColor(UINTN color)
{
    ConOut->SetAttribute(ConOut, color);
}

void ResetColor()
{
    SetColor(EFI_LIGHTGRAY);
}

// TODO: Make better error handling
#define EFI_CALL(function)            \
    do                                \
    {                                 \
        EFI_STATUS status = function; \
        if (EFI_ERROR(status))        \
        {                             \
            SetColor(EFI_RED);        \
            Print(L"Error");          \
            ResetColor();             \
        }                             \
    } while (0)

void InitializeEFI(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    ImageHandle = imageHandle;
    SystemTable = systemTable;

    ConOut = systemTable->ConOut;
    EFI_CALL(ConOut->Reset(ConOut, TRUE));

    ConIn = systemTable->ConIn;
    EFI_CALL(ConIn->Reset(ConIn, TRUE));

    BootServices = systemTable->BootServices;
    RuntimeServices = systemTable->RuntimeServices;
}

void SetCursorPosition(UINTN column, UINTN row)
{
    EFI_CALL(ConOut->SetCursorPosition(ConOut, column, row));
}

void WaitForKey()
{
    EFI_CALL(BootServices->WaitForEvent(1, &ConIn->WaitForKey, 0));
}

void Reboot()
{
    RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, 0);
}

void Restart()
{
    RuntimeServices->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, 0);
}

void Shutdown()
{
    RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, 0);
}
