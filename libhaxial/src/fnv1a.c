/**
 * @file fnv1a.c
 * @brief FNV-1a hash algorithm implementation
 */

#include "haxial/fnv1a.h"
#include <stdint.h>
#include <stddef.h>

/**
 * FNV-1a 32-bit prime (internal constant)
 */
#define HX_FNV1A_32_PRIME 0x01000193U

uint32_t hx_fnv1a_32(const void *data, size_t length, uint32_t seed) {
    if (data == NULL) {
        return seed;
    }
    
    const uint8_t *bytes = (const uint8_t *)data;
    uint32_t hash = seed;
    
    for (size_t i = 0; i < length; i++) {
        hash ^= bytes[i];
        hash *= HX_FNV1A_32_PRIME;  // FNV prime
    }
    
    return hash;
}

uint32_t hx_fnv1a_32_aligned(const void *data, size_t length, uint32_t seed) {
    if (data == NULL) {
        return seed;
    }
    
    const uint8_t *bytes = (const uint8_t *)data;
    uint32_t hash = seed;
    
    // Process unaligned prefix byte-by-byte
    uintptr_t addr = (uintptr_t)bytes;
    size_t unaligned = (-(intptr_t)addr) & 3U;
    
    if (unaligned > length) {
        unaligned = length;
    }
    
    for (size_t i = 0; i < unaligned; i++) {
        hash ^= bytes[i];
        hash *= HX_FNV1A_32_PRIME;
    }
    
    bytes += unaligned;
    length -= unaligned;
    
    // Process aligned 4-byte chunks
    size_t chunks = length >> 2;
    const uint32_t *words = (const uint32_t *)bytes;
    
    for (size_t i = 0; i < chunks; i++) {
        uint32_t word = words[i];
        
        // Process each byte of the word
        hash ^= (word & 0xFF);
        hash *= HX_FNV1A_32_PRIME;
        
        hash ^= ((word >> 8) & 0xFF);
        hash *= HX_FNV1A_32_PRIME;
        
        hash ^= ((word >> 16) & 0xFF);
        hash *= HX_FNV1A_32_PRIME;
        
        hash ^= ((word >> 24) & 0xFF);
        hash *= HX_FNV1A_32_PRIME;
    }
    
    // Process remaining bytes
    bytes += (chunks << 2);
    size_t remaining = length & 3U;
    
    for (size_t i = 0; i < remaining; i++) {
        hash ^= bytes[i];
        hash *= HX_FNV1A_32_PRIME;
    }
    
    return hash;
}

uint32_t hx_fnv1a_32_str(const char *str, uint32_t seed) {
    if (str == NULL) {
        return seed;
    }
    
    uint32_t hash = seed;
    
    while (*str != '\0') {
        hash ^= (uint8_t)(*str);
        hash *= HX_FNV1A_32_PRIME;
        str++;
    }
    
    return hash;
}
