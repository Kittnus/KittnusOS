#include "Graphics.h"

#include "Global.h"

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

void Graphics::Initialize()
{
  EFI_HANDLE* graphicsHandles;

  EFI_GUID GraphicsOutputProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  Global::BootServices->LocateHandleBuffer(
      ByProtocol, &GraphicsOutputProtocolGuid, NULL, nullptr, &graphicsHandles);
  Global::BootServices->HandleProtocol(graphicsHandles[0],
                                       &GraphicsOutputProtocolGuid,
                                       (void**)&Global::GraphicsOutput);

  auto graphicsInfo = Global::GraphicsOutput->Mode->Info;
  int width = graphicsInfo->HorizontalResolution;
  int height = graphicsInfo->VerticalResolution;

  m_OffsetX = (width - 80 * CHAR_WIDTH) / 2; // 80 characters per line
  m_OffsetY = (height - 24 * CHAR_HEIGHT) / 2; // 24 lines of text

  m_CenterX = width / 2;
  m_CenterY = height / 2;
}

void Graphics::Print(const wchar_t* string)
{
  Global::ConOut->OutputString(Global::ConOut, (CHAR16*)string);
}

void Graphics::ClearScreen() { Global::ConOut->ClearScreen(Global::ConOut); }
