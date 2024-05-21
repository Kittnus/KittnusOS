#pragma once

#include "Types.h"

class Graphics
{
private:
  static inline int s_OffsetX, s_OffsetY;
  static inline int s_CenterX, s_CenterY;

  static inline EFI_GRAPHICS_OUTPUT_PROTOCOL* s_GraphicsOutput;
public:
  static void Initialize();

  static void Print(const wchar_t* string);
  static void Print(UInt64 value);
  static void PrintLn(const wchar_t* string);
  static void PrintLn(UInt64 value);

  static void ClearScreen();
};
