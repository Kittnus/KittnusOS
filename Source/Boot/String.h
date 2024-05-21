#pragma once

#include "Types.h"

class String
{
public:
  static wchar_t* Concat(const wchar_t* string1, const wchar_t* string2);

  static UInt64 CalculateLength(const wchar_t* string);
};
