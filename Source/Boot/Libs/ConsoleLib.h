#pragma once

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

void SetColor(UINTN color)
{
    ConOut->SetAttribute(ConOut, color);
}

void ResetColor()
{
    SetColor(EFI_LIGHTGRAY);
}

void ClearScreen()
{
    EFI_CALL(ConOut->ClearScreen(ConOut));
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
