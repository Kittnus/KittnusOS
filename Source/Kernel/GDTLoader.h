#pragma once

#include "Types.h"

class GDTLoader
{
public:
  static void Load();
private:
  static void LoadSegments();
};
