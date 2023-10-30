#pragma once

#define ALLOC(buffer, size)                                                  \
    EFI_CALL(BootServices->AllocatePool(EfiBootServicesData, size, buffer)); \
    if (!*buffer)                                                            \
    {                                                                        \
        SetColor(EFI_RED);                                                   \
        Print(L"Failed to allocate memory for: ");                           \
        PrintLn(WIDEN(#buffer));                                             \
        ResetColor();                                                        \
    }

#define FREE(buffer) EFI_CALL(BootServices->FreePool(buffer));
