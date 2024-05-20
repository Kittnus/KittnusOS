#pragma once

class Graphics
{
private:
  static inline int m_OffsetX, m_OffsetY;
  static inline int m_CenterX, m_CenterY;
public:
  static void Initialize();

  static void Print(const char* string);
  static void ClearScreen();
};
