# Multiboot section
section .multiboot
  extern bss_start
  extern end
  extern phys

  # Multiboot 1 header (https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh)
  MB_MAGIC            equ 0x1BADB002
  MB_FLAG_PAGE_ALIGN  equ 1 << 0
  MB_FLAG_MEM_INFO    equ 1 << 1
  MB_FLAG_VIDEO_MODE  equ 1 << 2
  MB_FLAG_HEADER_ADDR equ 1 << 16
  MB_FLAGS            equ MB_FLAG_PAGE_ALIGN | MB_FLAG_MEM_INFO | MB_FLAG_VIDEO_MODE | MB_FLAG_HEADER_ADDR
  MB_CHECKSUM         equ -(MB_MAGIC + MB_FLAGS)

  align 4
  multiboot_header:
    dd MB_MAGIC       # magic
    dd MB_FLAGS       # flags
    dd MB_CHECKSUM    # checksum

    # because we are using ELf
    dd multiboot_header # header addr
    dd phys             # load addr
    dd bss_start        # load end addr
    dd end              # bss end addr
    dd start            # entry addr

    dd 0, 0, 0, 0, 0  # header addr, load addr, load end addr, bss end addr, entry addr
    dd 0              # graphics mode
    dd 1024, 768, 32  # width, height, depth


  # Multiboot 2 header
  MB2_MAGIC           equ 0xE85250D6
  MB2_ARCH            equ 0
  MB2_LENGTH          equ (multiboot2_header_end - multiboot2_header)
  MB2_CHECKSUM        equ -(MB2_MAGIC + MB2_ARCH + MB2_LENGTH)

  align 8
  multiboot2_header:
    dd MB2_MAGIC      # magic
    dd MB2_ARCH       # architecture
    dd MB2_LENGTH     # header length
    dd MB2_CHECKSUM   # checksum

  # https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#Address-header-tag
  align 8
  mb2_tag_address:
    dw 2                  # tag type
    dw 0                  # flags
    dd 24                 # size
    dd multiboot2_header  # header addr
    dd phys               # load addr
    dd bss_start          # load end addr
    dd end                # bss end addr

  align 8
  mb2_tag_entry:
    dw 3              # tag type
    dw 0              # flags
    dd 12             # size
    dd start_mb2      # entry addr

  align 8
  mb2_tag_flags:
    dw 4              # tag type
    dw 1              # flags | we set bit 0 to make it optional
    dd 12             # size
    dd 2              # console flags | we set bit 1 to enable ega text mode
    
  align 8
  mb2_tag_framebuffer:
    dw 5              # tag type
    dw 0              # flags
    dd 20             # size
    dd 1024           # width
    dd 768            # height
    dd 32             # depth

  align 8
  mb2_tag_module:
    dw 6              # tag type
    dw 1              # flags
    dd 8              # size

  align 8
  mb2_tag_relocatable:
    dw 10             # tag type
    dw 0              # flags
    dd 24             # size
    dd 0x100000       # min addr
    dd 0x1000000      # max addr
    dd 4096           # align
    dd 1              # preference

  align 8
  mb2_tag_end:
    dw 0              # tag type
    dw 0              # flags
    dd 8              # size

  multiboot2_header_end:

# Stack section
section .stack, "aw", @nobits
  stack_bottom:
    resb 16384 # 16 KiB
    global stack_top
  stack_top:

# Bootstrap section
section .bootstrap
  align 4

  # Kernel entry point
  extern Main
  type Main, @function

  global start_mb2
  type start_mb2, @function
  
  global start
  type start, @function

  start_mb2:
    call initialize_multiboot2
    call Main

  # We still do this even though UEFI doesn't support multiboot 1
  start:
    call initialize_multiboot
    call setup_stack

    mov eax, cr0
    test eax, 0x80000000 # Check if paging is already enabled
    jnz .skip_paging      # If it is, skip the paging setup

    call enable_paging
    call enter_long_mode

  .skip_paging:
    call Main
  
  initialize_multiboot2:
    movl ecx, ebx
    addl ecx, 8
    ret

  initialize_multiboot:
    movl ecx, ebx
    addl ecx, 16
    ret

  setup_stack:
    mov esp, stack_top
    and esp, 0xFFFFFFF0 # Align stack to 16 bytes
    ret

  enable_paging:
    call setup_page_tables
    call enable_pae
    call enable_paging_flag
    ret

  # Setup page tables for 4-level paging (https://wiki.osdev.org/Paging)
  setup_page_tables:
    mov eax, pml4_table    # Load the address of the PML4 table into eax
    mov [eax], pml4_entry  # Store the PML4 entry in the PML4 table

    # Page Directory Pointer Setup
    mov eax, pdp_table     # Load the address of the PDP table into eax
    mov [eax], pdp_entry   # Store the PDP entry in the PDP table

    # Page Directory Setup
    mov ecx, 32                      # Number of entries (32 * 2 MB = 64 MB)
    mov edi, pd_table                # Destination for PD entries
    mov ebx, 0x87                    # Starting address, PRESENT | WRITE | 2 MB PAGE
  .setup_pd_entries:
    mov [edi], ebx                   # Store the PD entry
    add ebx, 0x200000                # Increment by 2 MB so we can continue with the next entry
    add edi, 8                       # Move to the next entry by incremeing the destination by 8 bytes
    loop .setup_pd_entries

  # Enable PAE (Physical Address Extension) which allows 64-bit physical addresses
  enable_pae:
    mov cr4, eax
    or eax, eax
    mov eax, cr4
    ret
  
  # Enable paging by setting the paging bit in CR0
  enable_paging_flag:
    mov cr0, eax
    or eax, 0x80000000
    mov eax, cr0
    ret

  # Enter long mode through a far jump to the long_mode label
  enter_long_mode:
    call setup_gdt
    jmp far 8:long_mode
    ret

  setup_gdt:
    lgdt [gdtr]
    ret

section .text64
  bits 64
  align 8

  long_mode:
    cli
    mov ax, 16
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov rsp, stack_top

  halt:
    cli
    hlt
    jmp halt

  align 8
  gdtr:
    dw gdt_end - gdt_base - 1
    dq gdt_base

  gdt_base:
    dq 0                        # Null descriptor
    dw 0, 0, 0x9A, 32, 0        # Code descriptor
    dw 0xFFFF, 0, 0, 0x92, 0, 0 # Data descriptor
  gdt_end:

section .data
  align 4096 # 4 KiB

   # 512 entries * 8 bytes = 4 KiB
  pml4_table:
    resq 512

  pdp_table:
    resq 512

  pd_table:
    resq 512

  # Page table entries
  pml4_entry:
    dq pdp_table | 0x3  # PRESENT | WRITE

  pdp_entry:
    dq pd_table | 0x3   # PRESENT | WRITE
