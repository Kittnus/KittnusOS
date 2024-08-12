#pragma once

#include "Core/Types.h"

#define ELFCLASSNONE 0
#define ELFCLASS32   1
#define ELFCLASS64   2

struct ElfHeader
{
  UInt8 e_Ident[16];
  UInt16 e_Type;
  UInt16 e_Machine;
  UInt32 e_Version;
  UInt64 e_Entry;
  UInt64 e_PhdrOffset;
  UInt64 e_ShdrOffset;
  UInt32 e_Flags;
  UInt16 e_HeaderSize;
  UInt16 e_PhdrEntrySize;
  UInt16 e_PhdrCount;
  UInt16 e_ShdrEntrySize;
  UInt16 e_ShdrCount;
  UInt16 e_ShdrNamesIndex;
};

#define PT_NULL    0
#define PT_LOAD    1
#define PT_DYNAMIC 2
#define PT_INTERP  3
#define PT_NOTE    4
#define PT_SHLIB   5
#define PT_PHDR    6
#define PT_TLS     7

// https://docs.oracle.com/cd/E19683-01/816-1386/chapter6-83432/index.html
struct ElfProgramHeader
{
  UInt32 p_Type;
  UInt32 p_Flags;
  UInt64 p_Offset;
  UInt64 p_VAddr;
  UInt64 p_PAddr;
  UInt64 p_Filesz;
  UInt64 p_Memsz;
  UInt64 p_Align;
};

struct ElfSectionHeader
{
  UInt32 sh_Name;
  UInt32 sh_Type;
  UInt64 sh_Flags;
  UInt64 sh_Addr;
  UInt64 sh_Offset;
  UInt64 sh_Size;
  UInt32 sh_Link;
  UInt32 sh_Info;
  UInt64 sh_AddrAlign;
  UInt64 sh_EntrySize;
};

struct ElfRelocation
{
  UInt64 r_Offset;
  UInt64 r_Info;
};

struct ElfRelocationAddend
{
  UInt64 r_Offset;
  UInt64 r_Info;
  Int64 r_Addend;
};

struct ElfSymbol
{
  UInt32 st_Name;
  UInt8 st_Info;
  UInt8 st_Other;
  UInt16 st_SectionIndex;
  UInt64 st_Value;
  UInt64 st_Size;
};
