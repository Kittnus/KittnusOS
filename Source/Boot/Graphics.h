#pragma once

#include "EfiMacros.h"
#include "Font.h"
#include "Types.h"

class Graphics
{
private:
  static inline EFI_GRAPHICS_OUTPUT_PROTOCOL* s_GraphicsOutput;

  static inline UInt64 s_FrameBufferBase;
  static inline UInt64 s_FrameBufferSize;
  static inline UInt64 s_ScreenWidth;
  static inline UInt64 s_ScreenHeight;

  static inline UInt64 s_OffsetX = 0;
  static inline UInt64 s_OffsetY = 0;

  static inline UInt32 s_TextColor = 0xFF2F8E7B; // Greenish Cyan
  static inline UInt32 s_BackgroundColor = 0xFF000D16; // Dark Blueish Black

public:
  static void Initialize();

  static void Print(const char* string);
  static void Print(UInt64 value);
  static void PrintHex(UInt64 value);
  static void PrintLn(const char* string);
  static void PrintLn(UInt64 value);
  static void PrintHexLn(UInt64 value);

  static void ClearScreen();

private:
  static void FindGraphicsOutput();

  static void SetPixel(UInt64 x, UInt64 y, UInt32 color);
  static void DrawChar(char c, UInt64 x, UInt64 y);

  static void NewLine();
};
