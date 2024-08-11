#include "SerialPort.h"

#include "HAL/IO.h"

bool SerialPort::s_InUsePorts[4] = { false, false, false, false };

SerialPort::SerialPort(UInt16 port)
    : m_Com(port)
{
  if (IsInUse()) return;

  SetBaudRate(MAX_BAUD_RATE / BAUD_RATE);
  ConfigureLineControl();
  ConfigureFifoControl();
  ConfigureModemControl();

  SetInUse(true);
}

SerialPort::~SerialPort() { SetInUse(false); }

void SerialPort::Transmit(UInt8 byte)
{
  while (!IsTransmitEmpty());

  IO::Write8(DATA_PORT(m_Com), byte); // Send byte
}

void SerialPort::SetBaudRate(UInt16 divisor)
{
  IO::Write8(LINE_CONTROL_PORT(m_Com), 0b10000000); // Enable DLAB
  IO::Write8(DATA_PORT(m_Com), (divisor >> 8) & 0xFF); // Set divisor
  IO::Write8(DATA_PORT(m_Com), divisor & 0xFF); // Set divisor
}

void SerialPort::ConfigureLineControl()
{
  IO::Write8(LINE_CONTROL_PORT(m_Com),
             0b00000011); // 8N1 (8 data bits, no parity, 1 stop bit)
}

void SerialPort::ConfigureFifoControl()
{
  IO::Write8(FIFO_CONTROL_PORT(m_Com),
             0b11000111); // Enable FIFO, clear FIFO, set 14-byte threshold
}

void SerialPort::ConfigureModemControl()
{
  IO::Write8(MODEM_CONTROL_PORT(m_Com),
             0b00001011); // Enable DTR, RTS, and OUT2
}

bool SerialPort::IsTransmitEmpty()
{
  return IO::Read8(LINE_STATUS_PORT(m_Com)) & 0b00100000; // Check bit 5
}

int SerialPort::GetPortIndex()
{
  switch (m_Com)
  {
  case COM1:
    return 0;
  case COM2:
    return 1;
  case COM3:
    return 2;
  case COM4:
    return 3;
  default:
    return -1;
  }
}

bool SerialPort::IsInUse() { return s_InUsePorts[GetPortIndex()]; }

void SerialPort::SetInUse(bool inUse) { s_InUsePorts[GetPortIndex()] = inUse; }
