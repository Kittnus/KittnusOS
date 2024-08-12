#include "serial_port.h"

#include "hal/io.h"

static int ports_in_use[4] = { 0, 0, 0, 0 };

static void set_baud_rate(uint16_t port, uint16_t divisor);
static void configure_line_control(uint16_t port);
static void configure_fifo_control(uint16_t port);
static void configure_modem_control(uint16_t port);
static int is_transmit_empty(uint16_t port);
static int port_index(uint16_t port);
static int is_port_in_use(uint16_t port);
static void set_port_in_use(uint16_t port, int in_use);

void init_serial_port(uint16_t port)
{
  if (is_port_in_use(port)) return;

  set_baud_rate(port, MAX_BAUD_RATE / BAUD_RATE);
  configure_line_control(port);
  configure_fifo_control(port);
  configure_modem_control(port);

  set_port_in_use(port, 1);
}

void serial_transmit(uint16_t port, uint8_t byte)
{
  while (!is_transmit_empty(port));

  outb(port, byte);
}

static void set_baud_rate(uint16_t port, uint16_t divisor)
{
  outb(LINE_CONTROL_PORT(port), 0b10000000); // Enable DLAB
  outb(port, (divisor >> 8) & 0xFF);
  outb(port, divisor & 0xFF);
}

static void configure_line_control(uint16_t port)
{
  outb(LINE_CONTROL_PORT(port),
       0b00000011); // 8N1 (8 data bits, no parity, 1 stop bit)
}

static void configure_fifo_control(uint16_t port)
{
  outb(FIFO_CONTROL_PORT(port),
       0b11000111); // Enable FIFO, clear FIFO, set 14-byte threshold
}

static void configure_modem_control(uint16_t port)
{
  outb(MODEM_CONTROL_PORT(port), 0b00001011); // Enable DTR, RTS, and OUT2
}

static int is_transmit_empty(uint16_t port)
{
  return inb(LINE_STATUS_PORT(port)) & 0b00100000; // Check bit 5
}

static int port_index(uint16_t port)
{
  switch (port)
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

static int is_port_in_use(uint16_t port)
{
  int index = port_index(port);
  return index >= 0 ? ports_in_use[index] : 0;
}

static void set_port_in_use(uint16_t port, int in_use)
{
  int index = port_index(port);
  if (index >= 0) ports_in_use[index] = in_use;
}
