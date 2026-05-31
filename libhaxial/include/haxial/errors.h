/**
 * @file errors.h
 * @brief Error codes and constants for libhaxial
 * 
 * Error codes extracted from KDXServer binary analysis.
 * All error codes are uint32_t values.
 */

#ifndef HAXIAL_ERRORS_H
#define HAXIAL_ERRORS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Error code type
 */
typedef uint32_t TErrorCode;

/**
 * Error codes discovered from KDXServer analysis
 * 
 * These values are extracted from the binary and represent
 * the actual error codes used by the original KDX implementation.
 * 
 * Error code structure:
 *   0x1xxxx - System/general errors
 *   0x2xxxx - Memory/address errors
 *   0x3xxxx - File system errors
 *   0x4xxxx - Network/socket errors
 */

/* Success */
#define HX_ERROR_SUCCESS                0x00000000  /**< No error */

/* System/General errors (0x1xxxx) */
#define HX_ERROR_INVALID_PARAM          0x00010002  /**< Invalid parameter (EINVAL) */
#define HX_ERROR_ARGUMENT_TOO_LONG      0x0001000e  /**< Argument too long (E2BIG) */
#define HX_ERROR_NO_SUCH_PROCESS        0x00010012  /**< No such process (ESRCH) */
#define HX_ERROR_PERMISSION_DENIED      0x00010014  /**< Permission denied (EPERM/EACCES) */
#define HX_ERROR_RESOURCE_BUSY          0x00010016  /**< Resource busy (EBUSY) */
#define HX_ERROR_INVALID_MAGIC          0x00010019  /**< Invalid magic number */
#define HX_ERROR_UNKNOWN                0x0001001b  /**< Unknown error */

/* Memory/Address errors (0x2xxxx) */
#define HX_ERROR_OUT_OF_MEMORY          0x00020065  /**< Out of memory (ENOMEM) */
#define HX_ERROR_BAD_ADDRESS            0x00020066  /**< Bad address (EFAULT) */

/* File system errors (0x3xxxx) */
#define HX_ERROR_FILE_NOT_FOUND         0x00030066  /**< File not found (ENOENT) */
#define HX_ERROR_IS_DIRECTORY           0x0003006a  /**< Is a directory (EISDIR) */
#define HX_ERROR_NOT_DIRECTORY          0x0003006b  /**< Not a directory (ENOTDIR) */
#define HX_ERROR_READ_ONLY_FS           0x0003006e  /**< Read-only file system (EROFS) */
#define HX_ERROR_FILE_EXISTS            0x00030073  /**< File exists (EEXIST) */
#define HX_ERROR_NAME_TOO_LONG          0x00030076  /**< Filename too long (ENAMETOOLONG) */

/* Network/Socket errors (0x4xxxx) */
#define HX_ERROR_SOCKET_ALREADY_CLOSED  0x00040066  /**< Socket already closed */
#define HX_ERROR_ADDRESS_IN_USE         0x00040069  /**< Address in use (EADDRINUSE/EADDRNOTAVAIL) */
#define HX_ERROR_CONNECTION_RESET       0x0004006a  /**< Connection reset (ECONNRESET) */
#define HX_ERROR_CONNECTION_REFUSED     0x0004006b  /**< Connection refused (ECONNREFUSED) */
#define HX_ERROR_CONNECTION_TIMEOUT     0x0004006c  /**< Connection timeout (ETIMEDOUT) */
#define HX_ERROR_CONNECTION_ABORTED     0x0004006d  /**< Connection aborted (ENETRESET/ECONNABORTED/EHOSTDOWN) */
#define HX_ERROR_SOCKET_CLOSED          0x0004006e  /**< Socket closed */
#define HX_ERROR_NETWORK_UNREACHABLE    0x0004006f  /**< Network unreachable (ENETDOWN/ENETUNREACH/EHOSTUNREACH) */

/* Protocol/Packet errors (0x4xxxx high) */
#define HX_ERROR_INVALID_PACKET_MAGIC   0x00040071  /**< Invalid packet magic number */
#define HX_ERROR_SEQUENCE_ERROR         0x00040072  /**< Packet sequence error */
#define HX_ERROR_INVALID_HANDSHAKE      0x00040073  /**< Invalid handshake state */
#define HX_ERROR_PACKET_TOO_LARGE       0x00040075  /**< Packet too large (>2048 bytes) */

/**
 * Error code to string conversion
 * 
 * @param error Error code
 * @return String representation of error code
 */
const char* hx_error_string(TErrorCode error);

/**
 * Check if error code indicates success
 * 
 * @param error Error code to check
 * @return Non-zero if successful, zero if error
 */
static inline int hx_error_success(TErrorCode error) {
    return error == HX_ERROR_SUCCESS;
}

/**
 * Check if error code indicates failure
 * 
 * @param error Error code to check
 * @return Non-zero if failure, zero if success
 */
static inline int hx_error_failed(TErrorCode error) {
    return error != HX_ERROR_SUCCESS;
}

#ifdef __cplusplus
}
#endif

#endif /* HAXIAL_ERRORS_H */
