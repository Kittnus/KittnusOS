#include "log.h"

// TODO: Reimplement multiboot
void kernel_main()
{
  asm("cli");

  log_init(COM1);
  log_println("Kernel started");

  asm("sti");
}
