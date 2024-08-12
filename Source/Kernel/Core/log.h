#pragma once

#include "drivers/serial_port.h"

void log_init(uint16_t port);
void log_print(const char* string);
void log_println(const char* string);
