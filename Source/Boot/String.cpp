#include "String.h"

#include "Global.h"
#include "Memory.h"

wchar_t* String::Concat(const wchar_t* string1, const wchar_t* string2)
{
  auto length1 = CalculateLength(string1);
  auto length2 = CalculateLength(string2);
  auto length = length1 + length2;

  wchar_t* result;
  Memory::Allocate(length, (void**)&result);

  auto destination = result;
  while (*string1) *destination++ = *string1++;
  while (*string2) *destination++ = *string2++;
  *destination = 0;

  return result;
}

UInt64 String::CalculateLength(const wchar_t* string)
{
  auto length = 0ULL;

  while (*string++) length++;

  return length;
}
