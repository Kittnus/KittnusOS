; Multiboot section
section .multiboot
  extern bss_start
  extern end
  extern phys

  ; TODO: Add MultiBoot 1 support

  ; Multiboot 2 header
  MB2_MAGIC           equ 0xE85250D6
  MB2_ARCH            equ 0
  MB2_LENGTH          equ (multiboot2_header_end - multiboot2_header)
  MB2_CHECKSUM        equ -(MB2_MAGIC + MB2_ARCH + MB2_LENGTH)

  align 8
  multiboot2_header:
    dd MB2_MAGIC      ; magic
    dd MB2_ARCH       ; architecture
    dd MB2_LENGTH     ; header length
    dd MB2_CHECKSUM   ; checksum

  ; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#Address-header-tag
  align 8
  mb2_tag_address:
    dw 2                  ; tag type
    dw 0                  ; flags
    dd 24                 ; size
    dd multiboot2_header  ; header addr
    dd phys               ; load addr
    dd bss_start          ; load end addr
    dd end                ; bss end addr

  align 8
  mb2_tag_entry:
    dw 3              ; tag type
    dw 0              ; flags
    dd 12             ; size
    dd start      ; entry addr

  align 8
  mb2_tag_flags:
    dw 4              ; tag type
    dw 1              ; flags | we set bit 0 to make it optional
    dd 12             ; size
    dd 2              ; console flags | we set bit 1 to enable ega text mode
    
  align 8
  mb2_tag_framebuffer:
    dw 5              ; tag type
    dw 0              ; flags
    dd 20             ; size
    dd 1024           ; width
    dd 768            ; height
    dd 32             ; depth

  align 8
  mb2_tag_module:
    dw 6              ; tag type
    dw 1              ; flags
    dd 8              ; size

  align 8
  mb2_tag_relocatable:
    dw 10             ; tag type
    dw 0              ; flags
    dd 24             ; size
    dd 0x100000       ; min addr
    dd 0x1000000      ; max addr
    dd 4096           ; align
    dd 1              ; preference

  align 8
  mb2_tag_end:
    dw 0              ; tag type
    dw 0              ; flags
    dd 8              ; size

  multiboot2_header_end:

; Stack section
section .stack nobits alloc noexec write
  stack_bottom:
    resb 16384 ; 16 KiB
    global stack_top
  stack_top:

; Bootstrap section
section .bootstrap
  align 4

  ; Kernel entry point
  extern KernelEntry

  global start

  ; Only this function should be called from the "Kittnus Neo" bootloader
  start:
    mov ecx, ebx
    add ecx, 8
    call KernelEntry

section .data
