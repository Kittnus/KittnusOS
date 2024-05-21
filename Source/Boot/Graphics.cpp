#include "Graphics.h"

#include "Global.h"

#define CHAR_WIDTH  8
#define CHAR_HEIGHT 16

void Graphics::Initialize()
{
  EFI_HANDLE* graphicsHandles;

  // TODO: Better error handling
  EFI_GUID graphicsOutputProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  Global::BootServices->LocateHandleBuffer(
      ByProtocol, &graphicsOutputProtocolGuid, NULL, NULL, &graphicsHandles);
  Global::BootServices->HandleProtocol(graphicsHandles[0],
                                       &graphicsOutputProtocolGuid,
                                       (void**)&Global::GraphicsOutput);

  auto graphicsInfo = Global::GraphicsOutput->Mode->Info;
  auto width = graphicsInfo->HorizontalResolution;
  auto height = graphicsInfo->VerticalResolution;

  m_OffsetX = (width - 80 * CHAR_WIDTH) / 2; // 80 characters per line
  m_OffsetY = (height - 24 * CHAR_HEIGHT) / 2; // 24 lines of text

  m_CenterX = width / 2;
  m_CenterY = height / 2;
}

// TODO: Print using GOP
void Graphics::Print(const wchar_t* string)
{
  Global::ConOut->OutputString(Global::ConOut, (CHAR16*)string);
}

void Graphics::Print(UInt64 value)
{
  wchar_t buffer[17];
  buffer[16] = L'\0';

  for (int i = 0; i < 16; i++)
  {
    auto nibble = value & 0xF;
    buffer[15 - i] = nibble < 10 ? L'0' + nibble : L'A' + nibble - 10;
    value >>= 4;
  }

  Print(buffer);
}

void Graphics::PrintLn(const wchar_t* string)
{
  Print(string);
  Print(L"\n");
}

void Graphics::PrintLn(UInt64 value)
{
  Print(value);
  Print(L"\n");
}

// TODO: Clear the screen using GOP
void Graphics::ClearScreen() { Global::ConOut->ClearScreen(Global::ConOut); }
