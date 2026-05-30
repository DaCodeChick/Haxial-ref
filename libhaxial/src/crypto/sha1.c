/**
 * @file sha1.c
 * @brief SHA-1 implementation
 * 
 * Standard SHA-1 implementation for KDX protocol compatibility.
 */

#include "haxial/sha1.h"
#include <string.h>
#include <stdlib.h>

/**
 * Internal structure definition
 */
struct TSha1Context {
    uint32_t state[5];      /**< SHA-1 state values (h0-h4) */
    uint32_t block_count;   /**< Number of 64-byte blocks processed */
    uint8_t  buffer[64];    /**< Input buffer */
    uint32_t buffer_pos;    /**< Current position in buffer */
};

TSha1Context* hx_sha1_alloc(void) {
    return malloc(sizeof(TSha1Context));
}

void hx_sha1_free(TSha1Context *ctx) {
    free(ctx);
}

/* SHA-1 initial hash values */
#define SHA1_H0 0x67452301
#define SHA1_H1 0xEFCDAB89
#define SHA1_H2 0x98BADCFE
#define SHA1_H3 0x10325476
#define SHA1_H4 0xC3D2E1F0

/* SHA-1 functions */
#define SHA1_ROL(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

#define SHA1_BLK(i) (block[i & 15] = SHA1_ROL(block[(i + 13) & 15] ^ \
                                               block[(i + 8) & 15] ^ \
                                               block[(i + 2) & 15] ^ \
                                               block[i & 15], 1))

/* SHA-1 round functions */
#define SHA1_R0(v, w, x, y, z, i) \
    z += ((w & (x ^ y)) ^ y) + block[i] + 0x5A827999 + SHA1_ROL(v, 5); \
    w = SHA1_ROL(w, 30);

#define SHA1_R1(v, w, x, y, z, i) \
    z += ((w & (x ^ y)) ^ y) + SHA1_BLK(i) + 0x5A827999 + SHA1_ROL(v, 5); \
    w = SHA1_ROL(w, 30);

#define SHA1_R2(v, w, x, y, z, i) \
    z += (w ^ x ^ y) + SHA1_BLK(i) + 0x6ED9EBA1 + SHA1_ROL(v, 5); \
    w = SHA1_ROL(w, 30);

#define SHA1_R3(v, w, x, y, z, i) \
    z += (((w | x) & y) | (w & x)) + SHA1_BLK(i) + 0x8F1BBCDC + SHA1_ROL(v, 5); \
    w = SHA1_ROL(w, 30);

#define SHA1_R4(v, w, x, y, z, i) \
    z += (w ^ x ^ y) + SHA1_BLK(i) + 0xCA62C1D6 + SHA1_ROL(v, 5); \
    w = SHA1_ROL(w, 30);

/**
 * Transform one 64-byte block
 */
static void sha1_transform(uint32_t state[5], const uint8_t buffer[64]) {
    uint32_t a, b, c, d, e;
    uint32_t block[16];
    
    /* Copy buffer to block with byte swapping */
    for (int i = 0; i < 16; i++) {
        block[i] = ((uint32_t)buffer[i * 4 + 0] << 24) |
                   ((uint32_t)buffer[i * 4 + 1] << 16) |
                   ((uint32_t)buffer[i * 4 + 2] << 8) |
                   ((uint32_t)buffer[i * 4 + 3]);
    }
    
    /* Initialize working variables */
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    
    /* 4 rounds of 20 operations each */
    SHA1_R0(a, b, c, d, e,  0);
    SHA1_R0(e, a, b, c, d,  1);
    SHA1_R0(d, e, a, b, c,  2);
    SHA1_R0(c, d, e, a, b,  3);
    SHA1_R0(b, c, d, e, a,  4);
    SHA1_R0(a, b, c, d, e,  5);
    SHA1_R0(e, a, b, c, d,  6);
    SHA1_R0(d, e, a, b, c,  7);
    SHA1_R0(c, d, e, a, b,  8);
    SHA1_R0(b, c, d, e, a,  9);
    SHA1_R0(a, b, c, d, e, 10);
    SHA1_R0(e, a, b, c, d, 11);
    SHA1_R0(d, e, a, b, c, 12);
    SHA1_R0(c, d, e, a, b, 13);
    SHA1_R0(b, c, d, e, a, 14);
    SHA1_R0(a, b, c, d, e, 15);
    SHA1_R1(e, a, b, c, d, 16);
    SHA1_R1(d, e, a, b, c, 17);
    SHA1_R1(c, d, e, a, b, 18);
    SHA1_R1(b, c, d, e, a, 19);
    
    SHA1_R2(a, b, c, d, e, 20);
    SHA1_R2(e, a, b, c, d, 21);
    SHA1_R2(d, e, a, b, c, 22);
    SHA1_R2(c, d, e, a, b, 23);
    SHA1_R2(b, c, d, e, a, 24);
    SHA1_R2(a, b, c, d, e, 25);
    SHA1_R2(e, a, b, c, d, 26);
    SHA1_R2(d, e, a, b, c, 27);
    SHA1_R2(c, d, e, a, b, 28);
    SHA1_R2(b, c, d, e, a, 29);
    SHA1_R2(a, b, c, d, e, 30);
    SHA1_R2(e, a, b, c, d, 31);
    SHA1_R2(d, e, a, b, c, 32);
    SHA1_R2(c, d, e, a, b, 33);
    SHA1_R2(b, c, d, e, a, 34);
    SHA1_R2(a, b, c, d, e, 35);
    SHA1_R2(e, a, b, c, d, 36);
    SHA1_R2(d, e, a, b, c, 37);
    SHA1_R2(c, d, e, a, b, 38);
    SHA1_R2(b, c, d, e, a, 39);
    
    SHA1_R3(a, b, c, d, e, 40);
    SHA1_R3(e, a, b, c, d, 41);
    SHA1_R3(d, e, a, b, c, 42);
    SHA1_R3(c, d, e, a, b, 43);
    SHA1_R3(b, c, d, e, a, 44);
    SHA1_R3(a, b, c, d, e, 45);
    SHA1_R3(e, a, b, c, d, 46);
    SHA1_R3(d, e, a, b, c, 47);
    SHA1_R3(c, d, e, a, b, 48);
    SHA1_R3(b, c, d, e, a, 49);
    SHA1_R3(a, b, c, d, e, 50);
    SHA1_R3(e, a, b, c, d, 51);
    SHA1_R3(d, e, a, b, c, 52);
    SHA1_R3(c, d, e, a, b, 53);
    SHA1_R3(b, c, d, e, a, 54);
    SHA1_R3(a, b, c, d, e, 55);
    SHA1_R3(e, a, b, c, d, 56);
    SHA1_R3(d, e, a, b, c, 57);
    SHA1_R3(c, d, e, a, b, 58);
    SHA1_R3(b, c, d, e, a, 59);
    
    SHA1_R4(a, b, c, d, e, 60);
    SHA1_R4(e, a, b, c, d, 61);
    SHA1_R4(d, e, a, b, c, 62);
    SHA1_R4(c, d, e, a, b, 63);
    SHA1_R4(b, c, d, e, a, 64);
    SHA1_R4(a, b, c, d, e, 65);
    SHA1_R4(e, a, b, c, d, 66);
    SHA1_R4(d, e, a, b, c, 67);
    SHA1_R4(c, d, e, a, b, 68);
    SHA1_R4(b, c, d, e, a, 69);
    SHA1_R4(a, b, c, d, e, 70);
    SHA1_R4(e, a, b, c, d, 71);
    SHA1_R4(d, e, a, b, c, 72);
    SHA1_R4(c, d, e, a, b, 73);
    SHA1_R4(b, c, d, e, a, 74);
    SHA1_R4(a, b, c, d, e, 75);
    SHA1_R4(e, a, b, c, d, 76);
    SHA1_R4(d, e, a, b, c, 77);
    SHA1_R4(c, d, e, a, b, 78);
    SHA1_R4(b, c, d, e, a, 79);
    
    /* Add working variables back into state */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

void hx_sha1_init(TSha1Context *ctx) {
    ctx->state[0] = SHA1_H0;
    ctx->state[1] = SHA1_H1;
    ctx->state[2] = SHA1_H2;
    ctx->state[3] = SHA1_H3;
    ctx->state[4] = SHA1_H4;
    ctx->block_count = 0;
    ctx->buffer_pos = 0;
}

void hx_sha1_update(TSha1Context *ctx, const uint8_t *data, size_t length) {
    if (data == NULL || length == 0) {
        return;
    }
    
    /* Process any buffered data first */
    if (ctx->buffer_pos > 0) {
        size_t space = 64 - ctx->buffer_pos;
        size_t copy = (length < space) ? length : space;
        
        memcpy(ctx->buffer + ctx->buffer_pos, data, copy);
        ctx->buffer_pos += copy;
        data += copy;
        length -= copy;
        
        if (ctx->buffer_pos == 64) {
            sha1_transform(ctx->state, ctx->buffer);
            ctx->block_count++;
            ctx->buffer_pos = 0;
        }
    }
    
    /* Process complete 64-byte blocks */
    while (length >= 64) {
        sha1_transform(ctx->state, data);
        ctx->block_count++;
        data += 64;
        length -= 64;
    }
    
    /* Buffer remaining data */
    if (length > 0) {
        memcpy(ctx->buffer + ctx->buffer_pos, data, length);
        ctx->buffer_pos += length;
    }
}

void hx_sha1_finalize(TSha1Context *ctx, uint8_t *digest) {
    uint64_t total_bits;
    size_t pad_len;
    uint8_t padding[64];
    
    /* Calculate total length in bits */
    total_bits = ((uint64_t)ctx->block_count * 512) + (ctx->buffer_pos * 8);
    
    /* Padding: append 0x80, then zeros, then length */
    padding[0] = 0x80;
    memset(padding + 1, 0, 63);
    
    /* Calculate padding length (must end with 8 bytes for length) */
    if (ctx->buffer_pos < 56) {
        pad_len = 56 - ctx->buffer_pos;
    } else {
        pad_len = 120 - ctx->buffer_pos;
    }
    
    hx_sha1_update(ctx, padding, pad_len);
    
    /* Append length in bits as big-endian 64-bit value */
    padding[0] = (uint8_t)(total_bits >> 56);
    padding[1] = (uint8_t)(total_bits >> 48);
    padding[2] = (uint8_t)(total_bits >> 40);
    padding[3] = (uint8_t)(total_bits >> 32);
    padding[4] = (uint8_t)(total_bits >> 24);
    padding[5] = (uint8_t)(total_bits >> 16);
    padding[6] = (uint8_t)(total_bits >> 8);
    padding[7] = (uint8_t)(total_bits);
    
    hx_sha1_update(ctx, padding, 8);
    
    /* Output digest as big-endian */
    for (int i = 0; i < 5; i++) {
        digest[i * 4 + 0] = (uint8_t)(ctx->state[i] >> 24);
        digest[i * 4 + 1] = (uint8_t)(ctx->state[i] >> 16);
        digest[i * 4 + 2] = (uint8_t)(ctx->state[i] >> 8);
        digest[i * 4 + 3] = (uint8_t)(ctx->state[i]);
    }
}

void hx_sha1_hash(const uint8_t *data, size_t length, uint8_t *digest) {
    TSha1Context ctx;
    hx_sha1_init(&ctx);
    hx_sha1_update(&ctx, data, length);
    hx_sha1_finalize(&ctx, digest);
}
