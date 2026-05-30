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
 * Pascal string structure (255 bytes max)
 */
typedef struct HxPString {
    uint8_t length;
    char data[HX_PSTRING_MAX_LENGTH];
} HxPString;

/**
 * Initialize Pascal string from C string
 * 
 * @param pstr Destination Pascal string
 * @param cstr Source C string (null-terminated)
 * @return Length of resulting Pascal string (0-255)
 */
uint8_t hx_pstring_from_cstr(HxPString *pstr, const char *cstr);

/**
 * Convert Pascal string to C string
 * 
 * @param cstr Destination buffer (must be at least 256 bytes for safety)
 * @param pstr Source Pascal string
 * @param max_length Maximum bytes to write (including null terminator)
 * @return Number of characters written (excluding null terminator)
 */
size_t hx_pstring_to_cstr(char *cstr, const HxPString *pstr, size_t max_length);

/**
 * Copy Pascal string
 * 
 * @param dest Destination Pascal string
 * @param src Source Pascal string
 * @return Length of copied string
 */
uint8_t hx_pstring_copy(HxPString *dest, const HxPString *src);

/**
 * Copy Pascal string with truncation
 * 
 * @param dest Destination Pascal string
 * @param src Source Pascal string
 * @param max_length Maximum length to copy
 * @return Length of copied string (may be truncated)
 */
uint8_t hx_pstring_copy_truncate(HxPString *dest, const HxPString *src, uint8_t max_length);

/**
 * Copy Pascal string data to buffer (without length byte)
 * 
 * @param buffer Destination buffer
 * @param pstr Source Pascal string
 * @param max_length Maximum bytes to copy
 * @return Number of bytes copied
 */
uint8_t hx_pstring_to_buffer(void *buffer, const HxPString *pstr, uint8_t max_length);

/**
 * Get length of Pascal string
 * 
 * @param pstr Pascal string
 * @return Length (0-255)
 */
static inline uint8_t hx_pstring_length(const HxPString *pstr) {
    return (pstr != NULL) ? pstr->length : 0;
}

/**
 * Get pointer to data portion of Pascal string
 * 
 * @param pstr Pascal string
 * @return Pointer to data (not null-terminated)
 */
static inline const char* hx_pstring_data(const HxPString *pstr) {
    return (pstr != NULL) ? pstr->data : NULL;
}

/**
 * Check if Pascal string is empty
 * 
 * @param pstr Pascal string
 * @return true if empty or NULL
 */
static inline bool hx_pstring_is_empty(const HxPString *pstr) {
    return (pstr == NULL) || (pstr->length == 0);
}

/**
 * Compare two Pascal strings
 * 
 * @param pstr1 First Pascal string
 * @param pstr2 Second Pascal string
 * @return 0 if equal, <0 if pstr1 < pstr2, >0 if pstr1 > pstr2
 */
int hx_pstring_compare(const HxPString *pstr1, const HxPString *pstr2);

/**
 * Compare Pascal string with C string
 * 
 * @param pstr Pascal string
 * @param cstr C string
 * @return 0 if equal, <0 if pstr < cstr, >0 if pstr > cstr
 */
int hx_pstring_compare_cstr(const HxPString *pstr, const char *cstr);

#ifdef __cplusplus
}
#endif

#endif /* HAXIAL_PSTRING_H */
