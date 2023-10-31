#include "EFI/Types.h"
#include "EFI/Errors.h"
#include "EFI/API.h"

#include "Globals.h"
#include "Config.h"

#include "Libs/BaseLib.h"
#include "Libs/MemoryLib.h"
#include "Libs/StringLib.h"
#include "Libs/ConsoleLib.h"
#include "Libs/FileSystemLib.h"
#include "Libs/SystemLib.h"

#include "Shell.h"

EFI_STATUS Boot()
{
    WriteFile(L"Boot.log", "Hello, World!");

    return EFI_SUCCESS;
}

// TODO: Port to C++
EFI_STATUS EFIMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    InitializeLibs(imageHandle, systemTable);

    if (ShouldStartInShell)
        OpenShell();

    return Boot();
}
