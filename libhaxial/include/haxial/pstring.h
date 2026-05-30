/**
 * @file pstring.h
 * @brief Pascal string utilities
 * 
 * Pascal strings have a 1-byte length prefix followed by string data.
 * Maximum length is 255 bytes. No null terminator.
 * 
 * Format: [uint8_t length][char data[length]]
 * 
 * Example: "Hello" = [0x05, 'H', 'e', 'l', 'l', 'o']
 */

#ifndef HAXIAL_PSTRING_H
#define HAXIAL_PSTRING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Maximum length of a Pascal string (excluding length byte)
 */
#define HX_PSTRING_MAX_LENGTH 255

/**
 * Pascal string structure (opaque)
 * 
 * Use hx_pstring_* functions to access. Maximum length is 255 bytes.
 * 
 * For stack allocation, use HX_PSTRING_SIZE with aligned storage:
 *   alignas(TPascalString) char buf[HX_PSTRING_SIZE];
 *   TPascalString *pstr = (TPascalString *)buf;
 */
typedef struct TPascalString TPascalString;

/**
 * Size of TPascalString structure for stack allocation
 */
#define HX_PSTRING_SIZE 256

/**
 * Initialize Pascal string from C string
 * 
 * @param pstr Destination Pascal string
 * @param cstr Source C string (null-terminated)
 * @return Length of resulting Pascal string (0-255)
 */
uint8_t hx_pstring_from_cstr(TPascalString *pstr, const char *cstr);

/**
 * Convert Pascal string to C string
 * 
 * @param cstr Destination buffer (must be at least 256 bytes for safety)
 * @param pstr Source Pascal string
 * @param max_length Maximum bytes to write (including null terminator)
 * @return Number of characters written (excluding null terminator)
 */
size_t hx_pstring_to_cstr(char *cstr, const TPascalString *pstr, size_t max_length);

/**
 * Copy Pascal string
 * 
 * @param dest Destination Pascal string
 * @param src Source Pascal string
 * @return Length of copied string
 */
uint8_t hx_pstring_copy(TPascalString *dest, const TPascalString *src);

/**
 * Copy Pascal string with truncation
 * 
 * @param dest Destination Pascal string
 * @param src Source Pascal string
 * @param max_length Maximum length to copy
 * @return Length of copied string (may be truncated)
 */
uint8_t hx_pstring_copy_truncate(TPascalString *dest, const TPascalString *src, uint8_t max_length);

/**
 * Copy Pascal string data to buffer (without length byte)
 * 
 * @param buffer Destination buffer
 * @param pstr Source Pascal string
 * @param max_length Maximum bytes to copy
 * @return Number of bytes copied
 */
uint8_t hx_pstring_to_buffer(void *buffer, const TPascalString *pstr, uint8_t max_length);

/**
 * Get length of Pascal string
 * 
 * @param pstr Pascal string
 * @return Length (0-255)
 */
uint8_t hx_pstring_length(const TPascalString *pstr);

/**
 * Get pointer to data portion of Pascal string
 * 
 * @param pstr Pascal string
 * @return Pointer to data (not null-terminated)
 */
const char* hx_pstring_data(const TPascalString *pstr);

/**
 * Check if Pascal string is empty
 * 
 * @param pstr Pascal string
 * @return true if empty or NULL
 */
bool hx_pstring_is_empty(const TPascalString *pstr);

/**
 * Compare two Pascal strings
 * 
 * @param pstr1 First Pascal string
 * @param pstr2 Second Pascal string
 * @return 0 if equal, <0 if pstr1 < pstr2, >0 if pstr1 > pstr2
 */
int hx_pstring_compare(const TPascalString *pstr1, const TPascalString *pstr2);

/**
 * Compare Pascal string with C string
 * 
 * @param pstr Pascal string
 * @param cstr C string
 * @return 0 if equal, <0 if pstr < cstr, >0 if pstr > cstr
 */
int hx_pstring_compare_cstr(const TPascalString *pstr, const char *cstr);

#ifdef __cplusplus
}
#endif

#endif /* HAXIAL_PSTRING_H */
