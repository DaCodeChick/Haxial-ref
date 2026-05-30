/**
 * @file pstring_test.c
 * @brief Tests for Pascal string functions
 */

#include "haxial/pstring.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

static void test_from_cstr(void) {
    printf("Testing hx_pstring_from_cstr...\n");
    
    HxPString pstr;
    
    // Normal string
    uint8_t len = hx_pstring_from_cstr(&pstr, "Hello");
    assert(len == 5);
    assert(pstr.length == 5);
    assert(memcmp(pstr.data, "Hello", 5) == 0);
    
    // Empty string
    len = hx_pstring_from_cstr(&pstr, "");
    assert(len == 0);
    assert(pstr.length == 0);
    
    // NULL string
    len = hx_pstring_from_cstr(&pstr, NULL);
    assert(len == 0);
    assert(pstr.length == 0);
    
    // Long string (should truncate)
    char long_str[300];
    memset(long_str, 'A', 299);
    long_str[299] = '\0';
    
    len = hx_pstring_from_cstr(&pstr, long_str);
    assert(len == 255);
    assert(pstr.length == 255);
    
    printf("  PASS\n");
}

static void test_to_cstr(void) {
    printf("Testing hx_pstring_to_cstr...\n");
    
    HxPString pstr;
    char cstr[256];
    
    // Normal conversion
    hx_pstring_from_cstr(&pstr, "KDX Server");
    size_t len = hx_pstring_to_cstr(cstr, &pstr, sizeof(cstr));
    assert(len == 10);
    assert(strcmp(cstr, "KDX Server") == 0);
    
    // Empty Pascal string
    pstr.length = 0;
    len = hx_pstring_to_cstr(cstr, &pstr, sizeof(cstr));
    assert(len == 0);
    assert(cstr[0] == '\0');
    
    // Truncation
    hx_pstring_from_cstr(&pstr, "Long string");
    len = hx_pstring_to_cstr(cstr, &pstr, 6);  // Room for 5 chars + null
    assert(len == 5);
    assert(strcmp(cstr, "Long ") == 0);
    
    printf("  PASS\n");
}

static void test_copy(void) {
    printf("Testing hx_pstring_copy...\n");
    
    HxPString src, dest;
    
    hx_pstring_from_cstr(&src, "Test String");
    uint8_t len = hx_pstring_copy(&dest, &src);
    
    assert(len == 11);
    assert(dest.length == src.length);
    assert(memcmp(dest.data, src.data, src.length) == 0);
    
    // Copy from NULL
    len = hx_pstring_copy(&dest, NULL);
    assert(len == 0);
    assert(dest.length == 0);
    
    printf("  PASS\n");
}

static void test_copy_truncate(void) {
    printf("Testing hx_pstring_copy_truncate...\n");
    
    HxPString src, dest;
    
    hx_pstring_from_cstr(&src, "Long String Here");
    
    // Truncate to 10 chars
    uint8_t len = hx_pstring_copy_truncate(&dest, &src, 10);
    assert(len == 10);
    assert(dest.length == 10);
    assert(memcmp(dest.data, "Long Strin", 10) == 0);
    
    // No truncation needed
    len = hx_pstring_copy_truncate(&dest, &src, 255);
    assert(len == 16);
    assert(dest.length == 16);
    
    printf("  PASS\n");
}

static void test_to_buffer(void) {
    printf("Testing hx_pstring_to_buffer...\n");
    
    HxPString pstr;
    char buffer[256];
    
    hx_pstring_from_cstr(&pstr, "Buffer Test");
    uint8_t len = hx_pstring_to_buffer(buffer, &pstr, 255);
    
    assert(len == 11);
    assert(memcmp(buffer, "Buffer Test", 11) == 0);
    
    // Truncation
    len = hx_pstring_to_buffer(buffer, &pstr, 6);
    assert(len == 6);
    assert(memcmp(buffer, "Buffer", 6) == 0);
    
    printf("  PASS\n");
}

static void test_compare(void) {
    printf("Testing hx_pstring_compare...\n");
    
    HxPString pstr1, pstr2;
    
    // Equal strings
    hx_pstring_from_cstr(&pstr1, "test");
    hx_pstring_from_cstr(&pstr2, "test");
    assert(hx_pstring_compare(&pstr1, &pstr2) == 0);
    
    // Different strings
    hx_pstring_from_cstr(&pstr1, "abc");
    hx_pstring_from_cstr(&pstr2, "xyz");
    assert(hx_pstring_compare(&pstr1, &pstr2) < 0);
    assert(hx_pstring_compare(&pstr2, &pstr1) > 0);
    
    // Different lengths (prefix)
    hx_pstring_from_cstr(&pstr1, "test");
    hx_pstring_from_cstr(&pstr2, "testing");
    assert(hx_pstring_compare(&pstr1, &pstr2) < 0);
    
    printf("  PASS\n");
}

static void test_compare_cstr(void) {
    printf("Testing hx_pstring_compare_cstr...\n");
    
    HxPString pstr;
    
    // Equal
    hx_pstring_from_cstr(&pstr, "hello");
    assert(hx_pstring_compare_cstr(&pstr, "hello") == 0);
    
    // Different
    assert(hx_pstring_compare_cstr(&pstr, "world") < 0);
    assert(hx_pstring_compare_cstr(&pstr, "apple") > 0);
    
    // Different lengths
    assert(hx_pstring_compare_cstr(&pstr, "hello world") < 0);
    assert(hx_pstring_compare_cstr(&pstr, "hell") > 0);
    
    printf("  PASS\n");
}

static void test_helpers(void) {
    printf("Testing helper functions...\n");
    
    HxPString pstr;
    
    // Length
    hx_pstring_from_cstr(&pstr, "test");
    assert(hx_pstring_length(&pstr) == 4);
    
    // Data pointer
    const char *data = hx_pstring_data(&pstr);
    assert(memcmp(data, "test", 4) == 0);
    
    // Is empty
    assert(!hx_pstring_is_empty(&pstr));
    pstr.length = 0;
    assert(hx_pstring_is_empty(&pstr));
    assert(hx_pstring_is_empty(NULL));
    
    printf("  PASS\n");
}

static void test_kdx_strings(void) {
    printf("Testing KDX-specific strings...\n");
    
    HxPString pstr;
    
    // Common KDX strings
    hx_pstring_from_cstr(&pstr, "Public Chat");
    assert(pstr.length == 11);
    
    hx_pstring_from_cstr(&pstr, "Admin Lounge");
    assert(pstr.length == 12);
    
    hx_pstring_from_cstr(&pstr, "General News");
    assert(pstr.length == 12);
    
    hx_pstring_from_cstr(&pstr, "KDX");
    assert(pstr.length == 3);
    assert(memcmp(pstr.data, "KDX", 3) == 0);
    
    printf("  PASS\n");
}

int main(void) {
    printf("Running Pascal string tests...\n\n");
    
    test_from_cstr();
    test_to_cstr();
    test_copy();
    test_copy_truncate();
    test_to_buffer();
    test_compare();
    test_compare_cstr();
    test_helpers();
    test_kdx_strings();
    
    printf("\nAll Pascal string tests passed!\n");
    return 0;
}
