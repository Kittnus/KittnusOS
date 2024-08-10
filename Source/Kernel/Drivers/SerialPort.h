#pragma once

#include "Types.h"

#define COM1 0x3F8

#define BAUD_RATE    9600
#define BAUD_DIVISOR 115200 / BAUD_RATE

#define DATA_PORT          COM1
#define INTERRUPT_PORT     (COM1 + 1)
#define FIFO_CONTROL_PORT  (COM1 + 2)
#define LINE_CONTROL_PORT  (COM1 + 3)
#define MODEM_CONTROL_PORT (COM1 + 4)
#define LINE_STATUS_PORT   (COM1 + 5)
#define MODEM_STATUS_PORT  (COM1 + 6)
#define SCRATCH_PORT       (COM1 + 7)

class SerialPort
{
public:
  static void Initialize();

  static void Print(const char* data);
  static void PrintLn(const char* data);

private:
  static void WriteByte(UInt8 byte);
};
