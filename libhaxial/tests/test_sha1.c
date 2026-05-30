/**
 * @file test_sha1.c
 * @brief SHA-1 implementation tests
 */

#include <haxial.h>
#include <stdio.h>
#include <string.h>

/* Test vector from RFC 3174 */
static void test_sha1_basic(void) {
    const char *input = "abc";
    uint8_t digest[20];
    uint8_t expected[20] = {
        0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A,
        0xBA, 0x3E, 0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C,
        0x9C, 0xD0, 0xD8, 0x9D
    };
    
    hx_sha1_hash((const uint8_t *)input, strlen(input), digest);
    
    if (memcmp(digest, expected, 20) == 0) {
        printf("SHA-1 basic test: PASS\n");
    } else {
        printf("SHA-1 basic test: FAIL\n");
        printf("Expected: ");
        for (int i = 0; i < 20; i++) printf("%02X", expected[i]);
        printf("\nGot:      ");
        for (int i = 0; i < 20; i++) printf("%02X", digest[i]);
        printf("\n");
    }
}

/* Test empty string */
static void test_sha1_empty(void) {
    const char *input = "";
    uint8_t digest[20];
    uint8_t expected[20] = {
        0xDA, 0x39, 0xA3, 0xEE, 0x5E, 0x6B, 0x4B, 0x0D,
        0x32, 0x55, 0xBF, 0xEF, 0x95, 0x60, 0x18, 0x90,
        0xAF, 0xD8, 0x07, 0x09
    };
    
    hx_sha1_hash((const uint8_t *)input, strlen(input), digest);
    
    if (memcmp(digest, expected, 20) == 0) {
        printf("SHA-1 empty test: PASS\n");
    } else {
        printf("SHA-1 empty test: FAIL\n");
    }
}

/* Test incremental hashing */
static void test_sha1_incremental(void) {
    const char *part1 = "The quick brown fox ";
    const char *part2 = "jumps over the lazy dog";
    uint8_t digest1[20], digest2[20];
    TSha1Context ctx;
    
    /* All at once */
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%s%s", part1, part2);
    hx_sha1_hash((const uint8_t *)buffer, strlen(buffer), digest1);
    
    /* Incrementally */
    hx_sha1_init(&ctx);
    hx_sha1_update(&ctx, (const uint8_t *)part1, strlen(part1));
    hx_sha1_update(&ctx, (const uint8_t *)part2, strlen(part2));
    hx_sha1_finalize(&ctx, digest2);
    
    if (memcmp(digest1, digest2, 20) == 0) {
        printf("SHA-1 incremental test: PASS\n");
    } else {
        printf("SHA-1 incremental test: FAIL\n");
    }
}

int main(void) {
    printf("Running SHA-1 tests...\n\n");
    
    test_sha1_basic();
    test_sha1_empty();
    test_sha1_incremental();
    
    printf("\nSHA-1 tests complete.\n");
    return 0;
}
