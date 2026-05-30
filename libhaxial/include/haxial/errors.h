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
 */

/* Success */
#define HX_ERROR_SUCCESS                0x00000000  /**< No error */

/* General errors (0x40000 - 0x4FFFF) */
#define HX_ERROR_INVALID_PARAM          0x00040068  /**< Invalid parameter */
#define HX_ERROR_INVALID_MAGIC          0x00040069  /**< Invalid magic number */
#define HX_ERROR_OUT_OF_MEMORY          0x0004006a  /**< Memory allocation failed */

/* Socket errors (0x40000 - 0x4FFFF) */
#define HX_ERROR_SOCKET_CLOSED          0x0004006e  /**< Socket is closed */
#define HX_ERROR_SOCKET_ALREADY_CLOSED  0x0004006f  /**< Socket was already closed */

/* Authentication errors (0x40070 - 0x4007F) */
#define HX_ERROR_AUTH_FAILED            0x00040070  /**< Authentication failed */
#define HX_ERROR_AUTH_INVALID_USER      0x00040071  /**< Invalid username/password */
#define HX_ERROR_AUTH_BANNED            0x00040072  /**< User is banned */
#define HX_ERROR_AUTH_SERVER_ERROR      0x00040073  /**< Server authentication error */
#define HX_ERROR_AUTH_DUPLICATE_LOGIN   0x00040074  /**< Duplicate login attempt */

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
