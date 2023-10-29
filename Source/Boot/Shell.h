#pragma once

BOOLEAN IsInShell = FALSE;

typedef struct
{
    CHAR16 *commandName;
    CHAR16 *commandDescription;
    void *handler;
} CommandInfo;

void ShowHelp();
void ExitShell();

CommandInfo commands[] = {
    {L"Help", L"Show available commands", ShowHelp},
    {L"LoadKernel", L"Load the kernel", ExitShell},
    {L"Shutdown", L"Shut down the machine", Shutdown},
    {L"Restart", L"Restart the machine", Restart},
    {L"Reboot", L"Reboot the machine", Reboot}};

void ExecuteCommand(CHAR16 *command)
{
    for (UINTN i = 0; i < sizeof(commands) / sizeof(CommandInfo); i++)
        if (StrCmp(command, commands[i].commandName) == 0)
        {
            ((void (*)())commands[i].handler)();
            return;
        }

    Print(L"Unknown command: ");
    PrintLn(command);
}

void OpenShell()
{
    PrintLn(L"Welcome to Kittnus shell!");
    IsInShell = TRUE;
    while (IsInShell)
    {
        CHAR16 *command = ReadLn();
        ExecuteCommand(command);
    }
}

void ShowHelp()
{
    PrintLn(L"Commands:");
    for (UINTN i = 0; i < sizeof(commands) / sizeof(CommandInfo); i++)
    {
        SetCursorColumn(1);
        Print(commands[i].commandName);
        Print(L" - ");
        PrintLn(commands[i].commandDescription);
    }
}

void ExitShell()
{
    IsInShell = FALSE;
}
