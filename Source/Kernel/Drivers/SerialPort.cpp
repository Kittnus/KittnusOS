#include "SerialPort.h"

#include "HAL/IO.h"

SerialPort::SerialPort(UInt16 port)
    : m_Com(port)
{
  SetBaudRate(MAX_BAUD_RATE / BAUD_RATE);
  ConfigureLineControl();
  ConfigureFifoControl();
  ConfigureModemControl();

  // Here we configure IRQ4 (COM1 & COM3) or IRQ3 (COM2 & COM4)
}

void SerialPort::Print(const char* data)
{
  auto c = (char*)data;
  while (*c != 0) WriteByte((UInt8)*c++);
}

void SerialPort::PrintLn(const char* data)
{
  Print(data);
  Print("\r\n");
}

void SerialPort::SetBaudRate(UInt16 divisor)
{
  IO::Write8(LINE_CONTROL_PORT(m_Com), 0b10000000);
  IO::Write8(DATA_PORT(m_Com), (divisor >> 8) & 0xFF);
  IO::Write8(DATA_PORT(m_Com), divisor & 0xFF);
}

void SerialPort::ConfigureLineControl()
{
  IO::Write8(LINE_CONTROL_PORT(m_Com), 0b00000011);
}

void SerialPort::ConfigureFifoControl()
{
  IO::Write8(FIFO_CONTROL_PORT(m_Com), 0b11000111);
}

void SerialPort::ConfigureModemControl()
{
  IO::Write8(MODEM_CONTROL_PORT(m_Com), 0b00001011);
}

bool SerialPort::IsTransmitFifoEmpty()
{
  return IO::Read8(LINE_STATUS_PORT(m_Com)) & 0b00100000;
}

void SerialPort::WriteByte(UInt8 byte)
{
  while (!IsTransmitFifoEmpty());

  IO::Write8(DATA_PORT(m_Com), byte);
}
