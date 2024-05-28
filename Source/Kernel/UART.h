#pragma once

#include "Types.h"

#define COM 0x2F8

#define BAUD_RATE    9600
#define BAUD_DIVISOR 115200 / BAUD_RATE

#define DATA_PORT          COM
#define INTERRUPT_PORT     (COM + 1)
#define FIFO_CONTROL_PORT  (COM + 2)
#define LINE_CONTROL_PORT  (COM + 3)
#define MODEM_CONTROL_PORT (COM + 4)
#define LINE_STATUS_PORT   (COM + 5)
#define MODEM_STATUS_PORT  (COM + 6)
#define SCRATCH_PORT       (COM + 7)

class UART
{
public:
  static void Initialize();

  static void Print(const char* data);
  static void PrintLn(const char* data);

private:
  static void WriteByte(UInt8 byte);
};
