#include "UART.h"

#include "Memory.h"
#include "Port.h"
#include "String.h"

void UART::Initialize()
{
  Port::OutPortB(INTERRUPT_PORT, 0x00); // Disable all interrupts
  Port::OutPortB(LINE_CONTROL_PORT, 0b10000000); // Enable DLAB

  Port::OutPortB(DATA_PORT,
                 BAUD_DIVISOR); // Set baud rate to 9600 bps (low byte)
  Port::OutPortB(INTERRUPT_PORT,
                 BAUD_DIVISOR >> 8); // (high byte)

  Port::OutPortB(LINE_CONTROL_PORT,
                 0b00000011); // Disable DLAB, 8 bits, 1 stop bit, no parity
  Port::OutPortB(FIFO_CONTROL_PORT,
                 0b11000111); // Enable FIFO with 14 bytes threshold

  Port::OutPortB(MODEM_CONTROL_PORT, 0b00001111); // Enable RTS/DSR/DCD/RTS

  PrintLn("KittnusOS starting boot sequence..."); // TODO: Change this string

  Port::OutPortB(INTERRUPT_PORT,
                 0x01); // Enable received data available interrupt
}

void UART::Print(const char* data)
{
  auto c = (char*)data;
  while (*c != 0) WriteByte((UInt8)*c++);
}

void UART::PrintLn(const char* data)
{
  Print(data);
  Print("\r\n");
}

void UART::WriteByte(UInt8 byte) { Port::OutPortB(COM, byte); }
