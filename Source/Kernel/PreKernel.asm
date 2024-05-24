; Kernel entry point
extern KernelEntry

global start

; Only this function should be called from the "Kittnus Neo" bootloader
start:
  mov ecx, ebx
  add ecx, 8
  call KernelEntry
