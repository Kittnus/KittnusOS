#pragma once

#include "Types.h"

class Console
{
public:
  static void Print(const wchar_t* string);
  static void PrintLn(const wchar_t* string);

  static void ClearScreen();
};