#include "FileSystem.h"

#include "Global.h"
#include "Memory.h"
#include "StringUtils.h"

void FileSystem::Initialize()
{
  EFI_GUID loadedImageProtocolGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
  IF_ERROR_FATAL(Global::BootServices->HandleProtocol(Global::ImageHandle,
                                                      &loadedImageProtocolGuid,
                                                      (void**)&s_LoadedImage),
                 L"Failed to get loaded image protocol");

  EFI_GUID simpleFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
  IF_ERROR_FATAL(Global::BootServices->HandleProtocol(
                     s_LoadedImage->DeviceHandle, &simpleFileSystemProtocolGuid,
                     (void**)&s_SimpleFileSystem),
                 L"Failed to get simple file system protocol");

  IF_ERROR_FATAL(
      s_SimpleFileSystem->OpenVolume(s_SimpleFileSystem, &s_RootDirectory),
      L"Failed to open root directory");
}

EFI_FILE_PROTOCOL* FileSystem::OpenFile(const wchar_t* path, UInt64 mode)
{
  EFI_FILE_PROTOCOL* file;
  auto errorMessage = StringUtils::Concat(L"Failed to open file: ", path);
  IF_ERROR(
      s_RootDirectory->Open(s_RootDirectory, &file, (CHAR16*)path, mode, 0),
      errorMessage);

  Memory::Free(errorMessage);
  return file;
}
