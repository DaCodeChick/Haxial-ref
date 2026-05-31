/**
 * @file errno_convert.c
 * @brief Convert POSIX errno to KDX error codes
 * 
 * Implementation matches errno_to_kdx_error from KDXServer.lexe (0x08091820)
 */

#include "haxial/errors.h"
#include <errno.h>

/**
 * Convert POSIX errno to KDX error code
 * 
 * This function replicates the exact conversion logic from the original
 * KDXServer binary. Multiple errno values may map to the same KDX error
 * for error coalescing (e.g., all "network unreachable" errors -> one code).
 * 
 * @param errno_value POSIX errno value (e.g., ENOENT, ENOMEM, ECONNRESET)
 * @return Corresponding KDX error code
 */
TErrorCode hx_errno_to_error(int errno_value) {
    switch (errno_value) {
        /* Permission errors */
        case EPERM:     /* 1 */
        case EACCES:    /* 13 */
            return HX_ERROR_PERMISSION_DENIED;

        /* File/entry not found */
        case ENOENT:    /* 2 */
            return HX_ERROR_FILE_NOT_FOUND;

        /* Process errors */
        case ESRCH:     /* 3 */
            return HX_ERROR_NO_SUCH_PROCESS;

        /* Argument errors */
        case E2BIG:     /* 7 */
            return HX_ERROR_ARGUMENT_TOO_LONG;

        /* Memory errors */
        case ENOMEM:    /* 12 */
            return HX_ERROR_OUT_OF_MEMORY;

        case EFAULT:    /* 14 */
            return HX_ERROR_BAD_ADDRESS;

        /* Resource errors */
        case EBUSY:     /* 16 */
            return HX_ERROR_RESOURCE_BUSY;

        /* File system errors */
        case EEXIST:    /* 17 */
            return HX_ERROR_FILE_EXISTS;

        case ENOTDIR:   /* 20 */
            return HX_ERROR_NOT_DIRECTORY;

        case EISDIR:    /* 21 */
            return HX_ERROR_IS_DIRECTORY;

        case EINVAL:    /* 22 */
            return HX_ERROR_INVALID_PARAM;

        case EROFS:     /* 30 */
            return HX_ERROR_READ_ONLY_FS;

        case ENAMETOOLONG: /* 36 */
            return HX_ERROR_NAME_TOO_LONG;

        /* Network/Socket errors */
        case EADDRINUSE:     /* 98 */
        case EADDRNOTAVAIL:  /* 99 */
            return HX_ERROR_ADDRESS_IN_USE;

        case ENETDOWN:       /* 100 */
        case ENETUNREACH:    /* 101 */
        case EHOSTUNREACH:   /* 113 */
            return HX_ERROR_NETWORK_UNREACHABLE;

        case ENETRESET:      /* 102 */
        case ECONNABORTED:   /* 103 */
        case EHOSTDOWN:      /* 112 */
            return HX_ERROR_CONNECTION_ABORTED;

        case ECONNRESET:     /* 104 */
            return HX_ERROR_CONNECTION_RESET;

        case ETIMEDOUT:      /* 110 */
            return HX_ERROR_CONNECTION_TIMEOUT;

        case ECONNREFUSED:   /* 111 */
            return HX_ERROR_CONNECTION_REFUSED;

        /* Unknown/unmapped errors */
        default:
            return HX_ERROR_UNKNOWN;
    }
}
