#include "String.h"

wchar_t* StringUtils::Concat(const wchar_t* string1, const wchar_t* string2)
{
  auto length1 = CalculateLength(string1);
  auto length2 = CalculateLength(string2);
  auto length = length1 + length2;

  auto result = new wchar_t[length + 1];
  auto destination = result;

  while (*string1) *destination++ = *string1++;
  while (*string2) *destination++ = *string2++;

  *destination = 0;

  return result;
}

UInt64 StringUtils::CalculateLength(const wchar_t* string)
{
  auto length = 0ULL;

  while (*string++) length++;

  return length;
}
