#pragma once

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

UINTN StrLenUTF8(const char *string)
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

void StrLower(CHAR16 *string)
{
    UnicodeInterface->StrLwr(UnicodeInterface, string);
}

void StrUpper(CHAR16 *string)
{
    UnicodeInterface->StrUpr(UnicodeInterface, string);
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

    buffer[digits] = '\0';
    do
    {
        digits--;
        buffer[digits] = (CHAR16)(L'0' + (number % 10));
        number /= 10;
    } while (number != 0);

    return buffer;
}

// TODO: Fix
CHAR16 *GetVersionString()
{
    CHAR16 *version = 0;
    ALLOC((void **)&version, (18 + 1) * sizeof(CHAR16));

    StrCat(version, IntToString(VERSION_MAJOR));
    StrCat(version, L".");
    StrCat(version, IntToString(VERSION_MINOR));
    StrCat(version, L".");
    StrCat(version, IntToString(VERSION_PATCH));

    return version;
}
