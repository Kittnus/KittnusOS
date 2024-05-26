#include "FileSystem.h"

#include "Global.h"
#include "Memory.h"
#include "String.h"

void FileSystem::Initialize()
{
  EFI_GUID loadedImageProtocolGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
  EFI_CHECK(Global::BootServices->HandleProtocol(Global::ImageHandle,
                                                 &loadedImageProtocolGuid,
                                                 (void**)&s_LoadedImage),
            "Failed to get loaded image protocol");

  EFI_GUID simpleFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
  EFI_CHECK(Global::BootServices->HandleProtocol(s_LoadedImage->DeviceHandle,
                                                 &simpleFileSystemProtocolGuid,
                                                 (void**)&s_SimpleFileSystem),
            "Failed to get simple file system protocol");

  EFI_CHECK(
      s_SimpleFileSystem->OpenVolume(s_SimpleFileSystem, &s_RootDirectory),
      "Failed to open root directory");
}

EFI_FILE_PROTOCOL* FileSystem::OpenFile(const wchar_t* path, UInt64 mode)
{
  EFI_FILE_PROTOCOL* file;
  auto errorMessage =
      String::Concat("Failed to open file: ", String::Cast<char>(path));
  EFI_CHECK(
      s_RootDirectory->Open(s_RootDirectory, &file, (CHAR16*)path, mode, 0),
      errorMessage);

  Memory::Free(errorMessage);
  return file;
}
