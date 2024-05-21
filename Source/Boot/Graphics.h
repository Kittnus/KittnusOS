#pragma once

#include "Types.h"

class Graphics
{
private:
  static inline int m_OffsetX, m_OffsetY;
  static inline int m_CenterX, m_CenterY;

public:
  static void Initialize();

  static void Print(const wchar_t* string);
  static void Print(UInt64 value);
  static void PrintLn(const wchar_t* string);
  static void PrintLn(UInt64 value);

  static void ClearScreen();
};
