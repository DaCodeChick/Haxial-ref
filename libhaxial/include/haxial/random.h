/**
 * @file random.h
 * @brief Random number generation for libhaxial
 * 
 * Random ID generation matching KDXServer.lexe implementation.
 * Uses FNV-1a hash + LCG for generating random IDs.
 */

#ifndef HAXIAL_RANDOM_H
#define HAXIAL_RANDOM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get current time in microseconds
 * 
 * Returns the microseconds component of the current time.
 * On POSIX systems, uses gettimeofday().
 * 
 * @return Microseconds (0-999999)
 */
uint32_t hx_get_microseconds(void);

/**
 * Generate random number
 * 
 * Generates a random 32-bit number using the KDX algorithm:
 * - Hashes 256-byte random pool with FNV-1a (seeded with microseconds)
 * - XORs with LCG seed
 * - Updates pool with result
 * 
 * Thread-safe: uses internal mutex for pool access.
 * 
 * @return Random 32-bit number
 */
uint32_t hx_random(void);

/**
 * Seed the random number generator
 * 
 * Optional: seeds the RNG with a specific value.
 * If not called, the RNG auto-seeds on first use.
 * 
 * @param seed Initial seed value
 */
void hx_random_seed(uint32_t seed);

#ifdef __cplusplus
}
#endif

#endif /* HAXIAL_RANDOM_H */
