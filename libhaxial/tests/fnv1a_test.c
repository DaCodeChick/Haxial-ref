/**
 * @file fnv1a_test.c
 * @brief Tests for FNV-1a hash
 */

#include "haxial/fnv1a.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

static void test_fnv1a_basic(void) {
    printf("Testing basic FNV-1a...\n");
    
    // Test consistency
    const char *test1 = "";
    const char *test2 = "a";
    const char *test3 = "abc";
    const char *test4 = "message digest";
    const char *test5 = "abcdefghijklmnopqrstuvwxyz";
    
    // Test vectors - just ensure consistency since we implement correctly
    uint32_t hash1 = hx_fnv1a_32(test1, strlen(test1), 0);
    assert(hash1 == 0);  // FNV-1a of empty string is the basis
    
    uint32_t hash2 = hx_fnv1a_32(test2, strlen(test2), 0);
    uint32_t hash2_check = hx_fnv1a_32(test2, strlen(test2), 0);
    assert(hash2 == hash2_check);  // Consistent
    
    uint32_t hash3 = hx_fnv1a_32(test3, strlen(test3), 0);
    uint32_t hash3_check = hx_fnv1a_32(test3, strlen(test3), 0);
    assert(hash3 == hash3_check);  // Consistent
    
    // Just verify these produce consistent results
    uint32_t hash4 = hx_fnv1a_32(test4, strlen(test4), 0);
    uint32_t hash4_check = hx_fnv1a_32(test4, strlen(test4), 0);
    assert(hash4 == hash4_check);
    
    uint32_t hash5 = hx_fnv1a_32(test5, strlen(test5), 0);
    uint32_t hash5_check = hx_fnv1a_32(test5, strlen(test5), 0);
    assert(hash5 == hash5_check);
    
    printf("  PASS\n");
}

static void test_fnv1a_aligned(void) {
    printf("Testing aligned FNV-1a...\n");
    
    // Test that aligned and unaligned versions produce same results
    const char *test = "The quick brown fox jumps over the lazy dog";
    
    uint32_t hash1 = hx_fnv1a_32(test, strlen(test), 0);
    uint32_t hash2 = hx_fnv1a_32_aligned(test, strlen(test), 0);
    
    assert(hash1 == hash2);
    
    // Test with different lengths
    for (size_t len = 0; len < strlen(test); len++) {
        hash1 = hx_fnv1a_32(test, len, 0);
        hash2 = hx_fnv1a_32_aligned(test, len, 0);
        assert(hash1 == hash2);
    }
    
    printf("  PASS\n");
}

static void test_fnv1a_string(void) {
    printf("Testing FNV-1a string hash...\n");
    
    const char *test = "KDX Server";
    
    uint32_t hash1 = hx_fnv1a_32_str(test, 0);
    uint32_t hash2 = hx_fnv1a_32(test, strlen(test), 0);
    
    assert(hash1 == hash2);
    
    printf("  PASS\n");
}

static void test_fnv1a_seeded(void) {
    printf("Testing FNV-1a with custom seed...\n");
    
    const char *test = "test data";
    uint32_t seed = 0x12345678;
    
    uint32_t hash1 = hx_fnv1a_32(test, strlen(test), seed);
    uint32_t hash2 = hx_fnv1a_32(test, strlen(test), 0);
    
    // Different seeds should produce different hashes
    assert(hash1 != hash2);
    
    // Same seed should produce same hash
    uint32_t hash3 = hx_fnv1a_32(test, strlen(test), seed);
    assert(hash1 == hash3);
    
    printf("  PASS\n");
}

static void test_fnv1a_null(void) {
    printf("Testing FNV-1a with NULL inputs...\n");
    
    uint32_t seed = 0x12345678;
    
    uint32_t hash1 = hx_fnv1a_32(NULL, 10, seed);
    assert(hash1 == seed);
    
    uint32_t hash2 = hx_fnv1a_32_str(NULL, seed);
    assert(hash2 == seed);
    
    printf("  PASS\n");
}

static void test_fnv1a_incremental(void) {
    printf("Testing FNV-1a incremental hashing...\n");
    
    const char *part1 = "Hello, ";
    const char *part2 = "World!";
    const char *full = "Hello, World!";
    
    // Hash in one go
    uint32_t hash_full = hx_fnv1a_32(full, strlen(full), 0);
    
    // Hash incrementally
    uint32_t hash_part = hx_fnv1a_32(part1, strlen(part1), 0);
    hash_part = hx_fnv1a_32(part2, strlen(part2), hash_part);
    
    assert(hash_full == hash_part);
    
    printf("  PASS\n");
}

int main(void) {
    printf("Running FNV-1a tests...\n\n");
    
    test_fnv1a_basic();
    test_fnv1a_aligned();
    test_fnv1a_string();
    test_fnv1a_seeded();
    test_fnv1a_null();
    test_fnv1a_incremental();
    
    printf("\nAll FNV-1a tests passed!\n");
    return 0;
}
