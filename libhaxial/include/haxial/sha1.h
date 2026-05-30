/**
 * @file sha1.h
 * @brief SHA-1 hash implementation for libhaxial
 * 
 * SHA-1 implementation reverse-engineered from KDXServer.
 * This is a standard SHA-1 implementation used for authentication.
 * 
 * Note: SHA-1 is cryptographically broken for collision resistance,
 * but we implement it for compatibility with the original KDX protocol.
 */

#ifndef HAXIAL_SHA1_H
#define HAXIAL_SHA1_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * SHA-1 context structure
 * 
 * Note: In the original binary, SHA-1 functions use a uint[22] array
 * plus padding rather than a formal struct. This struct documents the
 * layout for our implementation.
 * 
 * Layout (92 bytes):
 *   state[5]:      +0x00 (20 bytes) - SHA-1 state (h0-h4)
 *   block_count:   +0x14 (4 bytes)  - Number of 64-byte blocks processed
 *   buffer[64]:    +0x18 (64 bytes) - Input buffer
 *   buffer_pos:    +0x58 (4 bytes)  - Current position in buffer (0-64)
 */
typedef struct {
    uint32_t state[5];      /**< SHA-1 state values (h0-h4) */
    uint32_t block_count;   /**< Number of 64-byte blocks processed */
    uint8_t  buffer[64];    /**< Input buffer */
    uint32_t buffer_pos;    /**< Current position in buffer */
} TSha1Context;

/**
 * Initialize SHA-1 context
 * 
 * @param ctx SHA-1 context to initialize
 */
void hx_sha1_init(TSha1Context *ctx);

/**
 * Update SHA-1 hash with data
 * 
 * @param ctx SHA-1 context
 * @param data Data to hash
 * @param length Length of data in bytes
 */
void hx_sha1_update(TSha1Context *ctx, const uint8_t *data, size_t length);

/**
 * Finalize SHA-1 hash and produce digest
 * 
 * @param ctx SHA-1 context
 * @param digest Output buffer for 20-byte digest
 */
void hx_sha1_finalize(TSha1Context *ctx, uint8_t *digest);

/**
 * Convenience function: compute SHA-1 hash of data in one call
 * 
 * This is equivalent to calling init, update, and finalize.
 * Matches the sha1_hash function from KDXServer binary.
 * 
 * @param data Data to hash
 * @param length Length of data in bytes
 * @param digest Output buffer for 20-byte digest
 */
void hx_sha1_hash(const uint8_t *data, size_t length, uint8_t *digest);

#ifdef __cplusplus
}
#endif

#endif /* HAXIAL_SHA1_H */
