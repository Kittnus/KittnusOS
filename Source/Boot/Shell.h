#pragma once

BOOLEAN IsInShell = FALSE;

typedef struct
{
    CHAR16 *name;
    CHAR16 *description;
    void *handler;
} CommandInfo;

void ShowHelp();
void ExitShell();

// TODO: Add file system support
CommandInfo Commands[] = {
    {L"Help", L"Show available commands", ShowHelp},
    {L"Clear", L"Clear the screen", ClearScreen},
    {L"Boot", L"Boot the operating system", ExitShell},
    {L"Shutdown", L"Shut down the machine", Shutdown},
    {L"Restart", L"Restart the machine", Restart},
    {L"Reboot", L"Reboot the machine", Reboot}};

const UINT64 CommandCount = sizeof(Commands) / sizeof(CommandInfo);

void ExecuteCommand(CHAR16 *command)
{
    Trim(command);

    for (UINTN i = 0; i < CommandCount; i++)
        if (StrCmp(command, Commands[i].name) == 0)
        {
            ((void (*)())Commands[i].handler)();
            return;
        }

    Print(L"Unknown command: ");
    PrintLn(command);
}

void TabComplete(CHAR16 *buffer)
{
    // TODO: Add pretabbing (tabbing before typing anything and when there are multiple options)
    // TODO: Add tabbing after typing a few characters
    // TODO: Add file system tabbing

    if (!buffer)
        return;

    for (UINTN i = 0; i < CommandCount; i++)
        if (StrCmp(buffer, Commands[i].name) == 0)
        {
            buffer = Commands[i].name;
            Print(buffer);
            break;
        }
}

// TODO: Add command history
// TODO: Add command editing
// TODO: Add command completion
// TODO: Add command aliases
// TODO: Add command chaining
CHAR16 *ReadInput()
{
    SetColor(EFI_GREEN);
    Print(L"$ ");
    ResetColor();

    EFI_INPUT_KEY key;
    CHAR16 *buffer = 0;
    const UINTN bufferSize = 256;

    ALLOC((void **)&buffer, bufferSize * sizeof(CHAR16));
    if (!buffer)
    {
        SetColor(EFI_RED);
        PrintLn(L"Failed to allocate memory for command buffer");
        ResetColor();
        return 0;
    }

    UINTN index = 0;
    while (TRUE)
    {
        WaitForKey();
        ReadKeyStroke(&key);

        if (key.UnicodeChar == L'\r')
        {
            if (index <= 0)
                continue;

            buffer[index] = L'\0';
            break;
        }
        else if (key.UnicodeChar == L'\b')
        {
            if (index <= 0)
                continue;

            index--;
            buffer[index] = L'\0';
            Print(L"\b \b");
        }
        else if (key.UnicodeChar == L'\e')
            ExitShell();
        else if (key.UnicodeChar == L'\t')
            TabComplete(buffer);
        else if (key.UnicodeChar == L'\0')
            continue;
        else if (index < bufferSize - 1)
        {
            buffer[index++] = key.UnicodeChar;
            Print(&key.UnicodeChar);
        }
    }

    NewLine();
    return buffer;
}

void OpenShell()
{
    SetColor(EFI_WHITE);
    PrintLn(L"Welcome to Kittnus shell!");
    ResetColor();
    NewLine();

    IsInShell = TRUE;
    while (IsInShell)
    {
        CHAR16 *command = ReadInput();
        if (command)
        {
            ExecuteCommand(command);
            FREE(command);
        }
    }
}

void ShowHelp()
{
    PrintLn(L"Commands:");
    for (UINTN i = 0; i < CommandCount; i++)
    {
        SetCursorColumn(1);
        Print(Commands[i].name);
        Print(L" - ");
        PrintLn(Commands[i].description);
    }
}

void ExitShell()
{
    IsInShell = FALSE;
}
