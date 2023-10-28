#pragma once

#include "API.h"
#include <stdarg.h>

void PrintLn(CHAR16 *format, ...);

#define EFI_CALL(function)                   \
    do                                       \
    {                                        \
        EFI_STATUS status = function;        \
        if (status != EFI_SUCCESS)           \
            PrintLn(L"Error: %d\n", status); \
    } while (0)

EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;

EFI_BOOT_SERVICES *BootServices;
EFI_RUNTIME_SERVICES *RuntimeServices;

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;

void InitializeLibs(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    ImageHandle = imageHandle;
    SystemTable = systemTable;

    BootServices = systemTable->BootServices;
    RuntimeServices = systemTable->RuntimeServices;

    ConOut = systemTable->ConOut;
    ConOut->Reset(ConOut, TRUE);

    EFI_CALL(ConOut->Reset(ConOut, TRUE));
    EFI_CALL(ConIn->Reset(ConIn, TRUE));
}

static void PrintInternal(CHAR16 *format, va_list args)
{
    for (CHAR16 *c = format; *c != '\0'; c++)
    {
        if (*c == '%')
        {
            switch (*(c++))
            {
            case 'd':
            {
                int value = va_arg(args, int);

                CHAR16 buffer[32];
                int i = 0;

                if (value < 0)
                {
                    buffer[i++] = (CHAR16)'-';
                    value = -value;
                }

                do
                {
                    buffer[i++] = (CHAR16)(L'0' + (value % 10));
                    value /= 10;
                } while (value > 0);

                while (i-- >= 0)
                    ConOut->OutputString(ConOut, buffer + i);

                break;
            }
            case 's':
            {
                CHAR16 *string = va_arg(args, CHAR16 *);

                while (*string != '\0')
                    ConOut->OutputString(ConOut, string++);

                break;
            }
            default:
                break;
            }
        }
        else
            ConOut->OutputString(ConOut, c);
    }
}

void Print(CHAR16 *format, ...)
{
    va_list args;
    va_start(args, format);
    PrintInternal(format, args);
    va_end(args);
}

void PrintLn(CHAR16 *format, ...)
{
    va_list args;
    va_start(args, format);
    PrintInternal(format, args);
    ConOut->OutputString(ConOut, L"\n\r");
    va_end(args);
}

void SetColor(UINTN color)
{
    EFI_CALL(ConOut->SetAttribute(ConOut, color));
}
