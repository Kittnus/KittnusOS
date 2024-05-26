#include "Console.h"

#include "Global.h"

void Console::Print(const wchar_t* string)
{
  Global::ConOut->OutputString(Global::ConOut, (CHAR16*)string);
}

void Console::PrintLn(const wchar_t* string)
{
  Print(string);
  Print(L"\n");
}

void Console::ClearScreen()
{
  Global::SystemTable->ConOut->ClearScreen(Global::SystemTable->ConOut);
}
