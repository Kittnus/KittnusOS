#pragma once

#include "Types.h"

enum class ConsoleColor : UInt8
{
  Black = 0,
  Blue = 1,
  Green = 2,
  Cyan = 3,
  Red = 4,
  Magenta = 5,
  Brown = 6,
  LightGray = 7,
  DarkGray = 8,
  LightBlue = 9,
  LightGreen = 10,
  LightCyan = 11,
  LightRed = 12,
  LightMagenta = 13,
  Yellow = 14,
  White = 15
};

class Console
{
private:
  static const UInt16 c_VGAMemory = 0xB8000;
  static const UInt16 c_Width = 80;
  static const UInt16 c_Height = 25;

  static inline UInt16 s_CursorX;
  static inline UInt16 s_CursorY;

  static inline ConsoleColor s_Color;

public:
  static void Print(const char* string);
  static void PrintLn(const char* string);

  static void Print(const char* string, ConsoleColor color);
  static void PrintLn(const char* string, ConsoleColor color);

private:
  static void PutChar(char c);

  static void SetColor(ConsoleColor color);
};
