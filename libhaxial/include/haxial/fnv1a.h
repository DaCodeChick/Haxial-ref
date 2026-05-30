/**
 * @file fnv1a.h
 * @brief FNV-1a hash algorithm
 * 
 * FNV (Fowler-Noll-Vo) is a fast, non-cryptographic hash function.
 * FNV-1a is a variant that XORs before multiplying.
 * 
 * Used in KDX for random number generation and possibly data integrity.
 */

#ifndef HAXIAL_FNV1A_H
#define HAXIAL_FNV1A_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Compute FNV-1a 32-bit hash of data
 * 
 * @param data Data to hash
 * @param length Length of data in bytes
 * @param seed Initial hash value (use HX_FNV1A_32_OFFSET_BASIS for standard FNV-1a)
 * @return 32-bit hash value
 */
uint32_t hx_fnv1a_32(const void *data, size_t length, uint32_t seed);

/**
 * Compute FNV-1a 32-bit hash of data (aligned version)
 * 
 * This is the version used in KDX server, which processes 4-byte chunks
 * when data is properly aligned for better performance.
 * 
 * @param data Data to hash
 * @param length Length of data in bytes
 * @param seed Initial hash value
 * @return 32-bit hash value
 */
uint32_t hx_fnv1a_32_aligned(const void *data, size_t length, uint32_t seed);

/**
 * Compute FNV-1a 32-bit hash of null-terminated string
 * 
 * @param str String to hash
 * @param seed Initial hash value
 * @return 32-bit hash value
 */
uint32_t hx_fnv1a_32_str(const char *str, uint32_t seed);

#ifdef __cplusplus
}
#endif

#endif /* HAXIAL_FNV1A_H */
