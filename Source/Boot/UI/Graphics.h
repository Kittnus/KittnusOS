#pragma once

#include "Core/Types.h"
#include "Font.h"
#include "Utilities/EfiMacros.h"

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

  static const UInt32 c_DefaultTextColor = 0xFF2F8E7B; // Greenish Cyan
  static const UInt32 c_DefaultBackgroundColor =
      0xFF000D16; // Dark Blueish Black

  static inline UInt32 s_TextColor = c_DefaultTextColor;
  static inline UInt32 s_TextBackgroundColor = c_DefaultBackgroundColor;
  static inline UInt32 s_BackgroundColor = c_DefaultBackgroundColor;

public:
  static void Initialize();

  // TODO: Make this scope based
  static void SetTextColor(UInt32 color);
  static void ResetTextColor();
  static void SetTextBackgroundColor(UInt32 color);
  static void ResetTextBackgroundColor();
  static void SetBackgroundColor(UInt32 color);
  static void ResetBackgroundColor();

  static void Print(const char* string);
  static void Print(UInt64 value);
  static void PrintHex(UInt64 value);
  static void PrintLn(const char* string);
  static void PrintLn(UInt64 value);
  static void PrintHexLn(UInt64 value);

  static void NewLine();

  static void ClearLine();
  static void ClearScreen();

private:
  static void FindGraphicsOutput();

  static UInt32 GetPixel(UInt64 x, UInt64 y);
  static void SetPixel(UInt64 x, UInt64 y, UInt32 color);

  static void DrawChar(char c, UInt64 x, UInt64 y);
};
