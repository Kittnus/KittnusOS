#include "GDTLoader.h"

#include "GDT.h"

void GDTLoader::Load()
{
  GDTDescriptor descriptor;
  descriptor.Size = sizeof(GDT) - 1;
  descriptor.Offset = (UInt64)&g_GDT;

  asm volatile("lgdt %0"
               :
               : "m"(descriptor)); // Load the GDT into the processor
  LoadSegments();
}

void GDTLoader::LoadSegments()
{
  // Load the data segment
  asm volatile("mov $0x10, %ax"); // Load 0x10 into AX
  asm volatile("mov %ax, %ds");
  asm volatile("mov %ax, %es");
  asm volatile("mov %ax, %fs");
  asm volatile("mov %ax, %gs");
  asm volatile("mov %ax, %ss");

  // Load the code segment
  asm volatile("mov $0x08, %ax"); // Load 0x08 into AX
  asm volatile("push %rax");
  asm volatile("retfq");
}
