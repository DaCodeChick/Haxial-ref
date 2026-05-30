/**
 * @file blowfish.h
 * @brief Blowfish encryption implementation for libhaxial
 * 
 * Blowfish cipher implementation reverse-engineered from KDXServer.
 * Used for connection encryption in the KDX protocol.
 */

#ifndef HAXIAL_BLOWFISH_H
#define HAXIAL_BLOWFISH_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Blowfish context structure
 * 
 * Size: 4168 bytes total
 * Layout from KDXServer analysis:
 *   p_array[18]:  +0x0000 (72 bytes)  - P-array for Feistel network
 *   s_boxes[4][256]: +0x0048 (4096 bytes) - Four S-boxes for F function
 */
typedef struct {
    uint32_t p_array[18];        /**< P-array (18 entries) */
    uint32_t s_box_0[256];       /**< S-box 0 */
    uint32_t s_box_1[256];       /**< S-box 1 */
    uint32_t s_box_2[256];       /**< S-box 2 */
    uint32_t s_box_3[256];       /**< S-box 3 */
} TBlowfishContext;

/**
 * Initialize Blowfish context with key
 * 
 * @param ctx Blowfish context to initialize
 * @param key Encryption key
 * @param key_len Length of key in bytes (1-56 bytes)
 * @return 0 on success, non-zero on error
 */
int hx_blowfish_init(TBlowfishContext *ctx, const uint8_t *key, size_t key_len);

/**
 * Encrypt 8-byte block
 * 
 * @param ctx Initialized Blowfish context
 * @param input Input block (8 bytes)
 * @param output Output block (8 bytes)
 */
void hx_blowfish_encrypt(TBlowfishContext *ctx, const uint8_t *input, uint8_t *output);

/**
 * Decrypt 8-byte block
 * 
 * @param ctx Initialized Blowfish context
 * @param input Input block (8 bytes)
 * @param output Output block (8 bytes)
 */
void hx_blowfish_decrypt(TBlowfishContext *ctx, const uint8_t *input, uint8_t *output);

/**
 * Encrypt buffer in ECB mode
 * 
 * Note: ECB mode is not secure for general use, but is used in KDX protocol.
 * Buffer length must be a multiple of 8 bytes.
 * 
 * @param ctx Initialized Blowfish context
 * @param data Data to encrypt (modified in-place)
 * @param length Length in bytes (must be multiple of 8)
 * @return 0 on success, non-zero on error
 */
int hx_blowfish_encrypt_ecb(TBlowfishContext *ctx, uint8_t *data, size_t length);

/**
 * Decrypt buffer in ECB mode
 * 
 * Buffer length must be a multiple of 8 bytes.
 * 
 * @param ctx Initialized Blowfish context
 * @param data Data to decrypt (modified in-place)
 * @param length Length in bytes (must be multiple of 8)
 * @return 0 on success, non-zero on error
 */
int hx_blowfish_decrypt_ecb(TBlowfishContext *ctx, uint8_t *data, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* HAXIAL_BLOWFISH_H */
