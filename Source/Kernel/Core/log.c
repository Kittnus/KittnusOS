#include "log.h"

// TODO: Implement some modules logic here
static uint16_t log_port = COM1;

void log_init(uint16_t port)
{
  log_port = port;
  init_serial_port(port);
}

void log_print(const char* string)
{
  while (*string) serial_transmit(log_port, *string++);
}

void log_println(const char* string)
{
  log_print(string);
  log_print("\r\n");
}
