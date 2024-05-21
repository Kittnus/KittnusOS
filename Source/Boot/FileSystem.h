#pragma once

#include "Types.h"

class FileSystem
{
private:
  static inline EFI_LOADED_IMAGE_PROTOCOL* s_LoadedImage;
  static inline EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* s_SimpleFileSystem;
  static inline EFI_FILE_PROTOCOL* s_RootDirectory;
public:
  static void Initialize();

  static EFI_FILE_PROTOCOL* OpenFile(const wchar_t* path, UInt64 mode);
};
