#pragma once

BOOLEAN IsInShell = FALSE;

typedef struct
{
    CHAR16 *name;
    CHAR16 *alias;
    CHAR16 *description;
    void *handler;
} CommandInfo;

void ShowHelp();
void ExitShell();

// TODO: Add file system support
CommandInfo Commands[] = {
    {L"Help", L"?", L"Show available commands", ShowHelp},
    {L"Clear", L"Cls", L"Clear the screen", ClearScreen},
    {L"Boot", L"Start", L"Boot the operating system", ExitShell},
    {L"Shutdown", L"Exit", L"Shut down the machine", Shutdown},
    {L"Restart", 0, L"Restart the machine", Restart},
    {L"Reboot", 0, L"Reboot the machine", Reboot}};

const UINT64 CommandCount = sizeof(Commands) / sizeof(CommandInfo);

BOOLEAN FindCommand(CHAR16 *name, CommandInfo **commandInfo)
{
    if (!name)
        return FALSE;

    for (UINTN i = 0; i < CommandCount; i++)
        if (StrCmp(name, Commands[i].name) == 0 || StrCmp(name, Commands[i].alias) == 0)
        {
            *commandInfo = &Commands[i];
            return TRUE;
        }

    return FALSE;
}

void ExecuteCommand(CHAR16 *command)
{
    StrTrim(command);

    if (!command)
        return;

    CommandInfo *commandInfo;
    if (FindCommand(command, &commandInfo))
    {
        ((void (*)())commandInfo->handler)();
        return;
    }

    SetColor(EFI_RED);
    Print(L"Unknown command: ");
    PrintLn(command);
    ResetColor();
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
            StrCpy(buffer, Commands[i].name);
            Print(buffer);
            break;
        }
}

// TODO: Add command history
// TODO: Add command editing
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
        Print(L"  ");
        Print(Commands[i].name);
        if (Commands[i].alias)
        {
            Print(L" (");
            Print(Commands[i].alias);
            Print(L")");
        }
        Print(L" - ");
        PrintLn(Commands[i].description);
    }
}

void ExitShell()
{
    IsInShell = FALSE;
}
