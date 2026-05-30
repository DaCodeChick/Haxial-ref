/**
 * @file errors.c
 * @brief Error code to string conversion
 */

#include "haxial/errors.h"

const char* hx_error_string(TErrorCode error) {
    switch (error) {
        case HX_ERROR_SUCCESS:
            return "Success";
        
        /* General errors */
        case HX_ERROR_INVALID_PARAM:
            return "Invalid parameter";
        case HX_ERROR_INVALID_MAGIC:
            return "Invalid magic number";
        case HX_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        
        /* Socket errors */
        case HX_ERROR_SOCKET_CLOSED:
            return "Socket is closed";
        case HX_ERROR_SOCKET_ALREADY_CLOSED:
            return "Socket was already closed";
        
        /* Authentication errors */
        case HX_ERROR_AUTH_FAILED:
            return "Authentication failed";
        case HX_ERROR_AUTH_INVALID_USER:
            return "Invalid username or password";
        case HX_ERROR_AUTH_BANNED:
            return "User is banned";
        case HX_ERROR_AUTH_SERVER_ERROR:
            return "Server authentication error";
        case HX_ERROR_AUTH_DUPLICATE_LOGIN:
            return "Duplicate login attempt";
        
        default:
            return "Unknown error";
    }
}
