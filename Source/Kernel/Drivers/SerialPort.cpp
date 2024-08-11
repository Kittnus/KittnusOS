#include "SerialPort.h"

#include "HAL/IO.h"

SerialPort::SerialPort(UInt16 port)
    : m_Com(port)
{
  std::lock_guard<std::mutex> lock(m_Mutex);
  if (m_InUsePorts.find(port) != m_InUsePorts.end())
    return;

  SetBaudRate(MAX_BAUD_RATE / BAUD_RATE);
  ConfigureLineControl();
  ConfigureFifoControl();
  ConfigureModemControl();
}

SerialPort::~SerialPort()
{
  std::lock_guard<std::mutex> lock(m_Mutex);
  m_InUsePorts.erase(m_Com);
}

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
