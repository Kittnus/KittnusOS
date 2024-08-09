#pragma once

#include "Memory.h"
#include "Types.h"

class String
{
public:
  template<typename T>
  static T* NumToHex(UInt64 value)
  {
    constexpr auto bufferSize = 18;

    T* buffer;
    Memory::Allocate(bufferSize + 1, (void**)&buffer); // 18 chars + terminator
    buffer[bufferSize] = (T)'\0';

    buffer[0] = (T)'0';
    buffer[1] = (T)'x';

    for (int i = 0; i < bufferSize - 2; i++)
    {
      auto nibble = value & 0xF;
      buffer[bufferSize - i - 1] =
          nibble < 10 ? (T)'0' + nibble : (T)'A' + nibble - 10;
      value >>= 4;
    }

    return buffer;
  }

  template<typename T>
  static T* NumToDec(UInt64 value)
  {
    constexpr auto bufferSize = 20;

    T* buffer;
    Memory::Allocate(bufferSize + 1, (void**)&buffer); // 20 chars + terminator
    buffer[bufferSize] = (T)'\0';

    auto index = bufferSize - 1;
    do {
      buffer[index--] = (T)'0' + value % 10;
      value /= 10;
    } while (value);

    return buffer + index + 1;
  }

  template<typename T>
  static T* Concat(const T* string1, const T* string2)
  {
    auto length1 = CalculateLength(string1);
    auto length2 = CalculateLength(string2);
    auto length = length1 + length2;

    char* result;
    Memory::Allocate(length, (void**)&result);

    auto destination = result;
    while (*string1) *destination++ = *string1++;
    while (*string2) *destination++ = *string2++;
    *destination = 0;

    return result;
  }

  template<typename T, typename F>
  static T* Cast(const F* string)
  {
    auto length = CalculateLength(string);
    T* result;
    Memory::Allocate(length, (void**)&result);

    auto destination = result;
    while (*string) *destination++ = (T)*string++;
    *destination = (T)'\0';

    return result;
  }

  template<typename T>
  static UInt64 CalculateLength(const T* string)
  {
    UInt64 length = 0;
    while (*string++) length++;
    return length;
  }
};
