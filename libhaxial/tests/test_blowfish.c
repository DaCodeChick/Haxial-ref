/**
 * @file test_blowfish.c
 * @brief Blowfish implementation tests
 */

#include <haxial.h>
#include <stdio.h>
#include <string.h>

/* Test vector from Bruce Schneier's test vectors */
static void test_blowfish_basic(void) {
    TBlowfishContext *ctx = hx_blowfish_alloc();
    const uint8_t key[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    uint8_t plaintext[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    uint8_t ciphertext[8];
    uint8_t decrypted[8];
    
    hx_blowfish_init(ctx, key, sizeof(key));
    hx_blowfish_encrypt(ctx, plaintext, ciphertext);
    hx_blowfish_decrypt(ctx, ciphertext, decrypted);
    
    if (memcmp(plaintext, decrypted, 8) == 0) {
        printf("Blowfish encrypt/decrypt test: PASS\n");
    } else {
        printf("Blowfish encrypt/decrypt test: FAIL\n");
        printf("Original:  ");
        for (int i = 0; i < 8; i++) printf("%02X ", plaintext[i]);
        printf("\nDecrypted: ");
        for (int i = 0; i < 8; i++) printf("%02X ", decrypted[i]);
        printf("\n");
    }
    
    hx_blowfish_free(ctx);
}

/* Test ECB mode */
static void test_blowfish_ecb(void) {
    TBlowfishContext *ctx = hx_blowfish_alloc();
    const uint8_t key[] = "testkey";
    uint8_t data[16] = {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
    };
    uint8_t original[16];
    
    memcpy(original, data, 16);
    
    hx_blowfish_init(ctx, key, sizeof(key) - 1);
    hx_blowfish_encrypt_ecb(ctx, data, 16);
    hx_blowfish_decrypt_ecb(ctx, data, 16);
    
    if (memcmp(data, original, 16) == 0) {
        printf("Blowfish ECB mode test: PASS\n");
    } else {
        printf("Blowfish ECB mode test: FAIL\n");
    }
    
    hx_blowfish_free(ctx);
}

/* Test key size validation */
static void test_blowfish_key_validation(void) {
    TBlowfishContext *ctx = hx_blowfish_alloc();
    uint8_t key[60] = {0};  /* Initialize to zero */
    int result;
    
    /* Too large key (> 56 bytes) */
    result = hx_blowfish_init(ctx, key, 60);
    if (result != 0) {
        printf("Blowfish key validation (too large): PASS\n");
    } else {
        printf("Blowfish key validation (too large): FAIL\n");
    }
    
    /* NULL key */
    result = hx_blowfish_init(ctx, NULL, 10);
    if (result != 0) {
        printf("Blowfish key validation (NULL): PASS\n");
    } else {
        printf("Blowfish key validation (NULL): FAIL\n");
    }
    
    hx_blowfish_free(ctx);
}

int main(void) {
    printf("Running Blowfish tests...\n\n");
    
    test_blowfish_basic();
    test_blowfish_ecb();
    test_blowfish_key_validation();
    
    printf("\nBlowfish tests complete.\n");
    return 0;
}
