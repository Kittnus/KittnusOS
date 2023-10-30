#pragma once

#define EFI_ERROR(a) (((INTN)a) < 0)

#define ERRBASE 0x8000000000000000ull
#define EFIERR(a) (ERRBASE | a)
#define EFIWARN(a) (a)

#define EFI_SUCCESS 0

#define EFI_LOAD_ERROR EFIERR(1)
#define EFI_INVALID_PARAMETER EFIERR(2)
#define EFI_UNSUPPORTED EFIERR(3)
#define EFI_BAD_BUFFER_SIZE EFIERR(4)
#define EFI_BUFFER_TOO_SMALL EFIERR(5)
#define EFI_NOT_READY EFIERR(6)
#define EFI_DEVICE_ERROR EFIERR(7)
#define EFI_WRITE_PROTECTED EFIERR(8)
#define EFI_OUT_OF_RESOURCES EFIERR(9)
#define EFI_VOLUME_CORRUPTED EFIERR(10)
#define EFI_VOLUME_FULL EFIERR(11)
#define EFI_NO_MEDIA EFIERR(12)
#define EFI_MEDIA_CHANGED EFIERR(13)
#define EFI_NOT_FOUND EFIERR(14)
#define EFI_ACCESS_DENIED EFIERR(15)
#define EFI_NO_RESPONSE EFIERR(16)
#define EFI_NO_MAPPING EFIERR(17)
#define EFI_TIMEOUT EFIERR(18)
#define EFI_NOT_STARTED EFIERR(19)
#define EFI_ALREADY_STARTED EFIERR(20)
#define EFI_ABORTED EFIERR(21)
#define EFI_ICMP_ERROR EFIERR(22)
#define EFI_TFTP_ERROR EFIERR(23)
#define EFI_PROTOCOL_ERROR EFIERR(24)
#define EFI_INCOMPATIBLE_VERSION EFIERR(25)
#define EFI_SECURITY_VIOLATION EFIERR(26)
#define EFI_CRC_ERROR EFIERR(27)
#define EFI_END_OF_MEDIA EFIERR(28)
#define EFI_END_OF_FILE EFIERR(31)
#define EFI_INVALID_LANGUAGE EFIERR(32)
#define EFI_COMPROMISED_DATA EFIERR(33)
#define EFI_IP_ADDRESS_CONFLICT EFIERR(34)
#define EFI_HTTP_ERROR EFIERR(35)

#define EFI_WARN_UNKNOWN_GLYPH EFIWARN(1)
#define EFI_WARN_DELETE_FAILURE EFIWARN(2)
#define EFI_WARN_WRITE_FAILURE EFIWARN(3)
#define EFI_WARN_BUFFER_TOO_SMALL EFIWARN(4)
#define EFI_WARN_STALE_DATA EFIWARN(5)
#define EFI_WARN_FILE_SYSTEM EFIWARN(6)
#define EFI_WARN_RESET_REQUIRED EFIWARN(7)

CHAR16 *GetErrorString(EFI_STATUS status)
{
    switch (status)
    {
    case EFI_SUCCESS:
        return L"Success";
    case EFI_LOAD_ERROR:
        return L"Load Error";
    case EFI_INVALID_PARAMETER:
        return L"Invalid Parameter";
    case EFI_UNSUPPORTED:
        return L"Unsupported";
    case EFI_BAD_BUFFER_SIZE:
        return L"Bad Buffer Size";
    case EFI_BUFFER_TOO_SMALL:
        return L"Buffer Too Small";
    case EFI_NOT_READY:
        return L"Not Ready";
    case EFI_DEVICE_ERROR:
        return L"Device Error";
    case EFI_WRITE_PROTECTED:
        return L"Write Protected";
    case EFI_OUT_OF_RESOURCES:
        return L"Out of Resources";
    case EFI_VOLUME_CORRUPTED:
        return L"Volume Corrupt";
    case EFI_VOLUME_FULL:
        return L"Volume Full";
    case EFI_NO_MEDIA:
        return L"No Media";
    case EFI_MEDIA_CHANGED:
        return L"Media Changed";
    case EFI_NOT_FOUND:
        return L"Not Found";
    case EFI_ACCESS_DENIED:
        return L"Access Denied";
    case EFI_NO_RESPONSE:
        return L"No Response";
    case EFI_NO_MAPPING:
        return L"No Mapping";
    case EFI_TIMEOUT:
        return L"Time Out";
    case EFI_NOT_STARTED:
        return L"Not Started";
    case EFI_ALREADY_STARTED:
        return L"Already Started";
    case EFI_ABORTED:
        return L"Aborted";
    case EFI_ICMP_ERROR:
        return L"ICMP Error";
    case EFI_TFTP_ERROR:
        return L"TFTP Error";
    case EFI_PROTOCOL_ERROR:
        return L"Protocol Error";
    case EFI_INCOMPATIBLE_VERSION:
        return L"Incompatible Version";
    case EFI_SECURITY_VIOLATION:
        return L"Security Violation";
    case EFI_CRC_ERROR:
        return L"CRC Error";
    case EFI_END_OF_MEDIA:
        return L"End of Media";
    case EFI_END_OF_FILE:
        return L"End of File";
    case EFI_INVALID_LANGUAGE:
        return L"Invalid Language";
    case EFI_COMPROMISED_DATA:
        return L"Compromised Data";
    case EFI_IP_ADDRESS_CONFLICT:
        return L"IP Address Conflict";
    case EFI_HTTP_ERROR:
        return L"HTTP Error";
    case EFI_WARN_UNKNOWN_GLYPH:
        return L"Warning Unknown Glyph";
    case EFI_WARN_DELETE_FAILURE:
        return L"Warning Delete Failure";
    case EFI_WARN_WRITE_FAILURE:
        return L"Warning Write Failure";
    case EFI_WARN_BUFFER_TOO_SMALL:
        return L"Warning Buffer Too Small";
    case EFI_WARN_STALE_DATA:
        return L"Warning Stale Data";
    case EFI_WARN_FILE_SYSTEM:
        return L"Warning File System";
    case EFI_WARN_RESET_REQUIRED:
        return L"Warning Reset Required";
    default:
        return L"Unknown Error";
    }
}