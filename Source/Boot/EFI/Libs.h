#pragma once

#include "API.h"

EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;

EFI_BOOT_SERVICES *BootServices;
EFI_RUNTIME_SERVICES *RuntimeServices;

EFI_GUID UnicodeInterfaceGuid = EFI_UNICODE_COLLATION_PROTOCOL2_GUID;
EFI_UNICODE_COLLATION_PROTOCOL *UnicodeInterface;

UINTN globalCursorColumn = 0;

void Print(CHAR16 *string)
{
    ConOut->OutputString(ConOut, string);
}

void SetCursorColumn(UINTN column);

void PrintLn(CHAR16 *string)
{
    SetCursorColumn(globalCursorColumn);
    Print(string);
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

    EFI_CALL(BootServices->LocateProtocol(&UnicodeInterfaceGuid, 0, (void **)&UnicodeInterface));
}

void SetCursorPosition(UINTN column, UINTN row)
{
    EFI_CALL(ConOut->SetCursorPosition(ConOut, column, row));
}

void SetCursorColumn(UINTN column)
{
    globalCursorColumn = column;
    SetCursorPosition(column, ConOut->Mode->CursorRow);
}

void ResetCursorColumn()
{
    SetCursorColumn(0);
}

void SetCursorRow(UINTN row)
{
    SetCursorPosition(ConOut->Mode->CursorColumn, row);
}

CHAR16 *ReadLn()
{
    Print(L"> ");

    EFI_INPUT_KEY key;
    CHAR16 *buffer = 0;
    UINTN bufferSize = 256;
    UINTN index = 0;

    while (1)
    {
        EFI_CALL(BootServices->WaitForEvent(1, &ConIn->WaitForKey, 0));
        EFI_CALL(ConIn->ReadKeyStroke(ConIn, &key));

        if (key.UnicodeChar == L'\r')
        {
            buffer[index] = L'\0';
            break;
        }
        else if (key.UnicodeChar == L'\b' && index > 0)
        {
            index--;
            buffer[index] = L'\0';
            Print(L"\b");
        }
        else if (index < bufferSize - 1)
        {
            buffer[index] = key.UnicodeChar;
            index++;
            buffer[index] = L'\0';
            Print(&key.UnicodeChar);
        }
    }

    PrintLn(L"");
    return buffer;
}

INTN StrCmp(CHAR16 *string1, CHAR16 *string2)
{
    return UnicodeInterface->StriColl(UnicodeInterface, string1, string2);
}

void ToLower(CHAR16 *string)
{
    UnicodeInterface->StrLwr(UnicodeInterface, string);
}

void ToUpper(CHAR16 *string)
{
    UnicodeInterface->StrUpr(UnicodeInterface, string);
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
