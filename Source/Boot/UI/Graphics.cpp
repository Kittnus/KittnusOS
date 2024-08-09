#include "Graphics.h"

#include "Console.h"
#include "Global.h"
#include "Macros.h"
#include "Memory.h"
#include "String.h"
#include "Types.h"

void Graphics::Initialize()
{
  FindGraphicsOutput();

  // If we have a valid GOP handle we can now get the mode information
  auto mode = s_GraphicsOutput->Mode;
  s_FrameBufferBase = (UInt64)s_GraphicsOutput->Mode->FrameBufferBase;
  s_FrameBufferSize = s_GraphicsOutput->Mode->FrameBufferSize;

  auto info = mode->Info;
  s_ScreenWidth = info->HorizontalResolution;
  s_ScreenHeight = info->VerticalResolution;

  ClearScreen();
}

void Graphics::SetTextColor(UInt32 color) { s_TextColor = color; }

void Graphics::ResetTextColor() { SetTextColor(c_DefaultTextColor); }

void Graphics::SetTextBackgroundColor(UInt32 color)
{
  s_TextBackgroundColor = color;
}

void Graphics::ResetTextBackgroundColor()
{
  SetTextBackgroundColor(s_BackgroundColor);
}

void Graphics::SetBackgroundColor(UInt32 color)
{
  for (UInt64 y = 0; y < s_ScreenHeight; y++)
    for (UInt64 x = 0; x < s_ScreenWidth; x++)
      if (GetPixel(x, y) == s_BackgroundColor) SetPixel(x, y, color);
  if (s_TextBackgroundColor == s_BackgroundColor) SetTextBackgroundColor(color);

  s_BackgroundColor = color;
}

void Graphics::ResetBackgroundColor()
{
  SetBackgroundColor(c_DefaultBackgroundColor);
}

void Graphics::Print(const char* string)
{
  while (*string)
  {
    DrawChar(*string++, s_OffsetX, s_OffsetY);
    s_OffsetX += FONT_WIDTH;

    if (s_OffsetX + FONT_WIDTH >= s_ScreenWidth) NewLine();
  }
}

void Graphics::Print(UInt64 value)
{
  auto buffer = String::NumToDec<char>(value);
  Print(buffer);
  Memory::Free(buffer);
}

void Graphics::PrintHex(UInt64 value)
{
  auto buffer = String::NumToHex<char>(value);
  Print(buffer);
  Memory::Free(buffer);
}

void Graphics::PrintLn(const char* string)
{
  Print(string);
  NewLine();
}

void Graphics::PrintLn(UInt64 value)
{
  Print(value);
  NewLine();
}

void Graphics::PrintHexLn(UInt64 value)
{
  PrintHex(value);
  NewLine();
}

void Graphics::ClearScreen()
{
  for (UInt64 y = 0; y < s_ScreenHeight; y++)
    for (UInt64 x = 0; x < s_ScreenWidth; x++)
      SetPixel(x, y, s_BackgroundColor);

  s_OffsetX = 0;
  s_OffsetY = 0;
}

void Graphics::FindGraphicsOutput()
{
  UInt64 handlesCount;
  EFI_HANDLE* handleBuffer;

  EFI_GUID graphicsOutputProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  CHECK_CONSOLE(Global::BootServices->LocateHandleBuffer(
                    ByProtocol, &graphicsOutputProtocolGuid, NULL,
                    &handlesCount, &handleBuffer)
                    == EFI_SUCCESS,
                "Failed to locate GOP handle buffer");

  for (UInt64 i = 0; i < handlesCount; i++)
    if (Global::BootServices->HandleProtocol(handleBuffer[i],
                                             &graphicsOutputProtocolGuid,
                                             (void**)&s_GraphicsOutput)
        == EFI_SUCCESS)
      break;

  CHECK_CONSOLE(s_GraphicsOutput, "Failed to locate GOP handle");
}

UInt32 Graphics::GetPixel(UInt64 x, UInt64 y)
{
  return *(UInt32*)(s_FrameBufferBase + (y * s_ScreenWidth + x) * 4);
}

void Graphics::SetPixel(UInt64 x, UInt64 y, UInt32 color)
{
  auto pixel = (UInt32*)(s_FrameBufferBase + (y * s_ScreenWidth + x) * 4);
  *pixel = color;
}

void Graphics::DrawChar(char c, UInt64 x, UInt64 y)
{
  auto drawArray = g_FontData[(UInt8)c];

  for (UInt64 i = 0; i < FONT_HEIGHT; i++)
    for (UInt64 j = 0; j < FONT_WIDTH; j++)
    {
      auto isTextPixel = drawArray[i][j];
      if (s_TextBackgroundColor == s_BackgroundColor && !isTextPixel) continue;

      auto color = isTextPixel ? s_TextColor : s_TextBackgroundColor;
      SetPixel(x + j, y + i, color);
    }
}

void Graphics::NewLine()
{
  s_OffsetX = 0;
  s_OffsetY += FONT_HEIGHT;
}
