#pragma once

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
// TODO: Add argument support
CommandInfo Commands[] = {
    {L"help", L"?", L"Show available commands", ShowHelp},
    {L"clear", L"cls", L"Clear the screen", ClearScreen},
    {L"boot", L"start", L"Boot the operating system", ExitShell},
    {L"shutdown", L"exit", L"Shut down the machine", Shutdown},
    {L"restart", 0, L"Restart the machine", Restart},
    {L"reboot", 0, L"Reboot the machine", Reboot}};

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

// TODO: Add file system tabbing
void TabComplete(CHAR16 *buffer)
{
    UINTN bufferLength = StrLen(buffer);

    // TODO: Add pretabbing (tabbing before typing anything and when the buffer is empty) to show file system contents
    if (bufferLength <= 0)
        return;

    // TODO: Add multiple option selection when similar commands are found
    BOOLEAN isCheckingAlias = FALSE;
    for (UINTN i = 0; i < CommandCount; i++)
    {
        CHAR16 *nameToCheck = (isCheckingAlias ? Commands[i].name : Commands[i].alias);
        if (!nameToCheck)
            continue;

        UINTN nameLength = StrLen(nameToCheck);

        CHAR16 *matchingPart = 0;
        ALLOC((void **)&matchingPart, (nameLength + 1) * sizeof(CHAR16));

        StrCpy(matchingPart, nameToCheck);
        StrSlice(matchingPart, bufferLength);

        if (StrCmp(buffer, matchingPart) == 0)
        {
            StrCpy(buffer, nameToCheck);
            UINTN bufferLength = StrLen(buffer);
            CHAR16 *commandComplete = 0;
            ALLOC((void **)&commandComplete, (nameLength - bufferLength + 1) * sizeof(CHAR16));

            StrCpy(commandComplete, nameToCheck);
            StrSlice(commandComplete, bufferLength);
            Print(commandComplete);
            FREE(commandComplete);
            return;
        }

        if (i == CommandCount - 1)
        {
            isCheckingAlias = TRUE;
            i = 0;
        }

        FREE(matchingPart);
    }
}

void PrintPrompt()
{
    SetColor(EFI_GREEN);
    Print(L"$ ");
    ResetColor();
}

// TODO: Add command history
// TODO: Add command editing
// TODO: Add command chaining
CHAR16 *ReadInput()
{
    PrintPrompt();

    EFI_INPUT_KEY key;
    CHAR16 *buffer = 0;
    const UINTN bufferSize = 256;

    ALLOC((void **)&buffer, bufferSize * sizeof(CHAR16));

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
        else if (key.UnicodeChar == L'\t')
            TabComplete(buffer);
        else if (key.UnicodeChar == L'\b')
        {
            if (index <= 0)
                continue;

            index--;
            buffer[index] = L'\0';
            Print(L"\b \b");
        }
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
    Print(L"Welcome to ");
    SetColor(EFI_LIGHTCYAN);
    Print(L"Kittnus Shell");
    ResetColor();
    Print(L" (");
    Print(GetVersionString());
    Print(L")");
    PrintLn(L"!");

    PrintLn(L"Type \"Help\" to see available commands");
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
