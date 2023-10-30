#pragma once

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

void Print(CHAR16 *string)
{
    ConOut->OutputString(ConOut, string);
}

void NewLine()
{
    Print(L"\r\n");
}

void PrintLn(CHAR16 *string)
{
    Print(string);
    NewLine();
}

void SetColor(UINTN color)
{
    ConOut->SetAttribute(ConOut, color);
}

void ResetColor()
{
    SetColor(EFI_LIGHTGRAY);
}

void PrintIntLn(UINT64 number);

#define EFI_CALL(function)            \
    do                                \
    {                                 \
        EFI_STATUS status = function; \
        if (EFI_ERROR(status))        \
        {                             \
            SetColor(EFI_RED);        \
            Print(L"Error: ");        \
            PrintIntLn(status);       \
            ResetColor();             \
        }                             \
    } while (0)

#define ALLOC(buffer, size) EFI_CALL(BootServices->AllocatePool(EfiBootServicesData, size, buffer));
#define FREE(buffer) EFI_CALL(BootServices->FreePool(buffer));

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

    LocateProtocol(&SimpleFileSystemProtocolGuid, (void **)&FileSystem);
}

void ClearScreen()
{
    EFI_CALL(ConOut->ClearScreen(ConOut));
}

CHAR16 *IntToString(UINT64 number)
{
    CHAR16 *buffer = 0;
    UINT64 temp = number;
    UINT32 digits = 0;

    do
    {
        temp /= 10;
        digits++;
    } while (temp != 0);

    ALLOC((void **)&buffer, (digits + 1) * sizeof(CHAR16));
    if (!buffer)
    {
        SetColor(EFI_RED);
        PrintLn(L"Failed to allocate memory for integer buffer");
        ResetColor();
        return 0;
    }

    buffer[digits] = '\0';
    do
    {
        digits--;
        buffer[digits] = (CHAR16)(L'0' + (number % 10));
        number /= 10;
    } while (number != 0);

    return buffer;
}

void PrintInt(UINT64 number)
{
    CHAR16 *buffer = IntToString(number);
    Print(buffer);
    FREE(buffer);
}

void PrintIntLn(UINT64 number)
{
    PrintInt(number);
    NewLine();
}

void SetCursorPosition(UINTN column, UINTN row)
{
    EFI_CALL(ConOut->SetCursorPosition(ConOut, column, row));
}

void SetCursorColumn(UINTN column)
{
    SetCursorPosition(column, ConOut->Mode->CursorRow);
}

void SetCursorRow(UINTN row)
{
    SetCursorPosition(ConOut->Mode->CursorColumn, row);
}

void StrCpy(CHAR16 *destination, CHAR16 *source)
{
    while (*source)
        *destination++ = *source++;
    *destination = L'\0';
}

UINTN StrLen(CHAR16 *string)
{
    UINTN length = 0;
    while (*string++)
        length++;
    return length;
}

void StrCat(CHAR16 *destination, CHAR16 *source)
{
    UINTN length = StrLen(destination);
    StrCpy(destination + length, source);
}

void StrSlice(CHAR16 *string, UINTN start)
{
    UINTN length = StrLen(string);

    if (start >= length)
    {
        string[0] = L'\0';
        return;
    }
    
    StrCpy(string, string + start);
}

void StrTrim(CHAR16 *string)
{
    UINTN length = StrLen(string);
    if (!length)
        return;

    UINTN start = 0;
    UINTN end = length - 1;

    while (string[start] == L' ' || string[start] == L'\t')
        start++;

    while (string[end] == L' ' || string[end] == L'\t')
        end--;

    StrCpy(string, string + start);
    string[end - start + 1] = L'\0';
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

void ReadKeyStroke(EFI_INPUT_KEY *key)
{
    EFI_CALL(ConIn->ReadKeyStroke(ConIn, key));
}

void WaitForEvent(EFI_EVENT event)
{
    EFI_CALL(BootServices->WaitForEvent(1, &event, 0));
}

void WaitForKey()
{
    WaitForEvent(ConIn->WaitForKey);
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
