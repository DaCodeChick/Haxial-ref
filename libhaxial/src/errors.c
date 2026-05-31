/**
 * @file errors.c
 * @brief Error code to string conversion
 */

#include "haxial/errors.h"

const char* hx_error_string(TErrorCode error) {
    switch (error) {
        case HX_ERROR_SUCCESS:
            return "Success";
        
        /* System/General errors */
        case HX_ERROR_INVALID_PARAM:
            return "Invalid parameter";
        case HX_ERROR_ARGUMENT_TOO_LONG:
            return "Argument too long";
        case HX_ERROR_NO_SUCH_PROCESS:
            return "No such process";
        case HX_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        case HX_ERROR_RESOURCE_BUSY:
            return "Resource busy";
        case HX_ERROR_INVALID_MAGIC:
            return "Invalid magic number";
        case HX_ERROR_UNKNOWN:
            return "Unknown error";
        
        /* Memory/Address errors */
        case HX_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case HX_ERROR_BAD_ADDRESS:
            return "Bad address";
        
        /* File system errors */
        case HX_ERROR_FILE_NOT_FOUND:
            return "File not found";
        case HX_ERROR_IS_DIRECTORY:
            return "Is a directory";
        case HX_ERROR_NOT_DIRECTORY:
            return "Not a directory";
        case HX_ERROR_READ_ONLY_FS:
            return "Read-only file system";
        case HX_ERROR_FILE_EXISTS:
            return "File exists";
        case HX_ERROR_NAME_TOO_LONG:
            return "Filename too long";
        
        /* Network/Socket errors */
        case HX_ERROR_SOCKET_ALREADY_CLOSED:
            return "Socket already closed";
        case HX_ERROR_ADDRESS_IN_USE:
            return "Address in use";
        case HX_ERROR_CONNECTION_RESET:
            return "Connection reset";
        case HX_ERROR_CONNECTION_REFUSED:
            return "Connection refused";
        case HX_ERROR_CONNECTION_TIMEOUT:
            return "Connection timeout";
        case HX_ERROR_CONNECTION_ABORTED:
            return "Connection aborted";
        case HX_ERROR_SOCKET_CLOSED:
            return "Socket closed";
        case HX_ERROR_NETWORK_UNREACHABLE:
            return "Network unreachable";
        
        /* Protocol/Packet errors */
        case HX_ERROR_INVALID_PACKET_MAGIC:
            return "Invalid packet magic number";
        case HX_ERROR_SEQUENCE_ERROR:
            return "Packet sequence error";
        case HX_ERROR_INVALID_HANDSHAKE:
            return "Invalid handshake state";
        case HX_ERROR_PACKET_TOO_LARGE:
            return "Packet too large";
        
        default:
            return "Unknown error code";
    }
}
