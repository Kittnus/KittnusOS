#include "Graphics.h"

#include "Console.h"
#include "Global.h"
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
  if (Global::BootServices->LocateHandleBuffer(
          ByProtocol, &graphicsOutputProtocolGuid, NULL, &handlesCount,
          &handleBuffer)
      != EFI_SUCCESS)
  {
    Console::PrintLn(L"Failed to locate GOP handle buffer");
    while (true);
  }

  for (UInt64 i = 0; i < handlesCount; i++)
    if (Global::BootServices->HandleProtocol(handleBuffer[i],
                                             &graphicsOutputProtocolGuid,
                                             (void**)&s_GraphicsOutput)
        == EFI_SUCCESS)
      break;

  if (s_GraphicsOutput != NULL) return;

  Console::PrintLn(L"Failed to locate GOP handle");
  while (true);
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
      if (drawArray[i][j]) SetPixel(x + j, y + i, s_TextColor);
}

void Graphics::NewLine()
{
  s_OffsetX = 0;
  s_OffsetY += FONT_HEIGHT;
}
