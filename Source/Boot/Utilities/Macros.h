#pragma once

#include "UI/Console.h"
#include "UI/Graphics.h"

#define CHECK(condition, message)                 \
  if (!(condition))                               \
  {                                               \
    Graphics::SetTextColor(0xFFFFFFFF);           \
    Graphics::SetTextBackgroundColor(0xFFFF0000); \
    Graphics::PrintLn(message);                   \
    Graphics::ResetTextColor();                   \
    Graphics::ResetTextBackgroundColor();         \
    while (true);                                 \
  }

#define CHECK_CONSOLE(condition, message) \
  if (!(condition))                       \
  {                                       \
    Console::PrintLn(L##message);         \
    while (true);                         \
  }
