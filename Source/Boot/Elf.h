#pragma once

#include "EFI.h"

#define ELF32_R_SYM(i)     ((i) >> 8)
#define ELF32_R_TYPE(i)    ((i) & 0xFF)
#define ELF32_R_INFO(s, t) (((s) << 8) + ((t) & 0xFF))

#define ELF32_ST_BIND(i)    ((i) >> 4)
#define ELF32_ST_TYPE(i)    ((i) & 0xF)
#define ELF32_ST_INFO(b, t) (((b) << 4) + ((t) & 0xF))

#define ELFMAG0 0x7F
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

using Elf32_Addr = UINT32;
using Elf32_Half = UINT16;
using Elf32_Off = UINT32;
using Elf32_Sword = INT32;
using Elf32_Word = UINT32;

struct Elf32Header
{
  UINT8 e_ident[16];
  Elf32_Half e_type;

  Elf32_Half e_machine;
  Elf32_Word e_version;
  Elf32_Addr e_entry;

  Elf32_Off e_phoff;
  Elf32_Off e_shoff;
  Elf32_Word e_flags;
  Elf32_Half e_ehsize;

  Elf32_Half e_phentsize;
  Elf32_Half e_phnum;
  Elf32_Half e_shentsize;
  Elf32_Half e_shnum;
  Elf32_Half e_shstrndx;
};

// e_type
#define ET_NONE   0
#define ET_REL    1
#define ET_EXEC   2
#define ET_DYN    3
#define ET_CORE   4
#define ET_LOPROC 0xFF0
#define ET_HIPROC 0xFFF

// e_machine
#define EM_NONE  0
#define EM_M32   1
#define EM_SPARC 2
#define EM_386   3
#define EM_68K   4
#define EM_88K   5
#define EM_860   7
#define EM_MIPS  8

// e_version
#define EV_NONE    0
#define EV_CURRENT 1

struct Elf32ProgramHeader
{
  Elf32_Word p_type;

  Elf32_Off p_offset;
  Elf32_Addr p_vaddr;
  Elf32_Addr p_paddr;

  Elf32_Word p_filesz;
  Elf32_Word p_memsz;

  Elf32_Word p_flags;

  Elf32_Word p_align;
};

// p_type
#define PT_NULL    0
#define PT_LOAD    1
#define PT_DYNAMIC 2
#define PT_INTERP  3
#define PT_NOTE    4
#define PT_SHLIB   5
#define PT_PHDR    6
#define PT_LOPROC  0x70000000
#define PT_HIPROC  0x7FFFFFFF

struct Elf32SectionHeader
{
  Elf32_Word sh_name;
  Elf32_Word sh_type;

  Elf32_Word sh_flags;
  Elf32_Addr sh_addr;

  Elf32_Off sh_offset;
  Elf32_Word sh_size;

  Elf32_Word sh_link;
  Elf32_Word sh_info;

  Elf32_Word sh_addralign;
  Elf32_Word sh_entsize;
};

// sh_type
#define SHT_NULL     0
#define SHT_PROGBITS 1
#define SHT_SYMTAB   2
#define SHT_STRTAB   3
#define SHT_RELA     4
#define SHT_HASH     5
#define SHT_DYNAMIC  6
#define SHT_NOTE     7
#define SHT_NOBITS   8
#define SHT_REL      9
#define SHT_SHLIB    10
#define SHT_DYNSYM   11
#define SHT_LOPROC   0x70000000
#define SHT_HIPROC   0x7FFFFFFF
#define SHT_LOUSER   0x80000000
#define SHT_HIUSER   0xFFFFFFFF

struct Elf32AuxiliaryVector
{
  Elf32_Word a_type;
  Elf32_Word a_val;
};

struct Elf32Symbol
{
  Elf32_Word st_name;
  Elf32_Addr st_value;
  Elf32_Word st_size;

  UINT8 st_info;
  UINT8 st_other;

  Elf32_Half st_shndx;
};

// st_info
#define STB_LOCAL  0
#define STB_GLOBAL 1
#define STB_WEAK   2
#define STB_NUM    3

#define STB_LOPROC 13
#define STB_HIPROC 15

// st_other
#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4
#define STT_COMMON  5
#define STT_TLS     6
#define STT_NUM     7

#define STT_LOPROC 13
#define STT_HIPROC 15

struct Elf32Rel
{
  Elf32_Addr r_offset;
  Elf32_Word r_info;
};

struct Elf32Dyn
{
  Elf32_Sword d_tag;
  union
  {
    Elf32_Word d_val;
    Elf32_Addr d_ptr;
    Elf32_Off d_off;
  } d_un;
};
