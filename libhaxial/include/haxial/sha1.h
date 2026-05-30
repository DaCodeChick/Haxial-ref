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
 * SHA-1 context structure (opaque - use provided functions)
 * 
 * For stack allocation:
 *   TSha1Context ctx;
 *   hx_sha1_init(&ctx);
 */
typedef struct TSha1Context TSha1Context;

/**
 * Allocate SHA-1 context on heap
 * 
 * @return Pointer to allocated context, or NULL on error
 */
TSha1Context* hx_sha1_alloc(void);

/**
 * Free SHA-1 context allocated with hx_sha1_alloc
 * 
 * @param ctx Context to free
 */
void hx_sha1_free(TSha1Context *ctx);

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
