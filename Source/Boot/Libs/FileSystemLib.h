#pragma once

EFI_FILE_PROTOCOL *OpenFile(CHAR16 *name, UINT64 mode)
{
    EFI_FILE_PROTOCOL *file = 0;
    EFI_CALL(RootFS->Open(RootFS, &file, name, mode, 0));
    return file;
}

void CloseFile(EFI_FILE_PROTOCOL *file)
{
    EFI_CALL(file->Close(file));
}

EFI_FILE_PROTOCOL *CreateFile(CHAR16 *name)
{
    return OpenFile(name, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE);
}

void DeleteFile(CHAR16 *name)
{
    EFI_FILE_PROTOCOL *file = OpenFile(name, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE);
    EFI_CALL(file->Delete(file));
    CloseFile(file);
}

char *ReadFile(CHAR16 *name)
{
    UINTN size = 0;
    EFI_FILE_PROTOCOL *file = OpenFile(name, EFI_FILE_MODE_READ);
    EFI_CALL(file->GetInfo(file, &FileInfoId, &size, 0));

    char *buffer = 0;
    ALLOC((void **)&buffer, size);

    EFI_CALL(file->Read(file, &size, buffer));
    CloseFile(file);

    return buffer;
}

void WriteFile(CHAR16 *name, char *buffer)
{
    UINTN size = StrLenUTF8(buffer);
    EFI_FILE_PROTOCOL *file = OpenFile(name, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE);
    EFI_CALL(file->Write(file, &size, buffer));
    CloseFile(file);
}

/*
// TODO: Implement
void MakeDirectory(CHAR16 *name)
{
}

// TODO: Implement
void DeleteDirectory(CHAR16 *name)
{
}

// TODO: Implement
CHAR16 **ListFiles()
{
}

// TODO: Implement
CHAR16 **ListDirectories()
{
}
*/
