#pragma once

#include "Types.h"

#include <mutex>
#include <unordered_set>

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

#define MAX_BAUD_RATE 115200
#define BAUD_RATE     9600

#define DATA_PORT(COM)          COM
#define INTERRUPT_PORT(COM)     (COM + 1)
#define FIFO_CONTROL_PORT(COM)  (COM + 2)
#define LINE_CONTROL_PORT(COM)  (COM + 3)
#define MODEM_CONTROL_PORT(COM) (COM + 4)
#define LINE_STATUS_PORT(COM)   (COM + 5)
#define MODEM_STATUS_PORT(COM)  (COM + 6)
#define SCRATCH_PORT(COM)       (COM + 7)

class SerialPort
{
private:
  UInt16 m_Com;
  std::unordered_set<UInt16> m_InUsePorts;
  std::mutex m_Mutex;

public:
  SerialPort(UInt16 port = COM1);
  ~SerialPort();

  void Transmit(UInt8 byte);

private:
  void SetBaudRate(UInt16 divisor);
  void ConfigureLineControl();
  void ConfigureFifoControl();
  void ConfigureModemControl();

  bool IsTransmitEmpty();
};
