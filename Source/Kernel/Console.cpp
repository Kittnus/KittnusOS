#include "Console.h"

void Console::Print(const char* string)
{
  while (*string) PutChar(*string++);
}

void Console::PrintLn(const char* string)
{
  Print(string);
  Print("\n");
}

void Console::Print(const char* string, ConsoleColor color)
{
  auto currentColor = s_Color;
  SetColor(color);
  Print(string);
  SetColor(currentColor);
}

void Console::PrintLn(const char* string, ConsoleColor color)
{
  Print(string, color);
  Print("\n");
}

void Console::PutChar(char c)
{
  if (c == '\n')
    s_CursorY++;
  else
  {
    auto offset = s_CursorY * c_Width + s_CursorX;
    auto vga = (UInt16*)c_VGAMemory;
    vga[offset] = (UInt16)s_Color << 8 | c;
    s_CursorX++;
  }
}

void Console::SetColor(ConsoleColor color) { s_Color = color; }
