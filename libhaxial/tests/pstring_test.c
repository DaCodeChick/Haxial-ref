/**
 * @file pstring_test.c
 * @brief Tests for Pascal string functions
 */

#include "haxial/pstring.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdalign.h>

static void test_basic(void) {
    printf("Testing basic Pascal string operations...\n");
    
    // Stack allocation
    alignas(max_align_t) char buf1[HX_PSTRING_SIZE];
    alignas(max_align_t) char buf2[HX_PSTRING_SIZE];
    TPascalString *pstr1 = (TPascalString *)buf1;
    TPascalString *pstr2 = (TPascalString *)buf2;
    
    // From C string
    uint8_t len = hx_pstring_from_cstr(pstr1, "Hello");
    assert(len == 5);
    assert(hx_pstring_length(pstr1) == 5);
    
    // To C string
    char cstr[256];
    size_t clen = hx_pstring_to_cstr(cstr, pstr1, sizeof(cstr));
    assert(clen == 5);
    assert(strcmp(cstr, "Hello") == 0);
    
    // Copy
    uint8_t copied = hx_pstring_copy(pstr2, pstr1);
    assert(copied == 5);
    assert(hx_pstring_compare(pstr1, pstr2) == 0);
    
    // Empty
    hx_pstring_from_cstr(pstr1, "");
    assert(hx_pstring_is_empty(pstr1));
    
    printf("  PASS\n");
}

static void test_kdx_strings(void) {
    printf("Testing KDX-specific strings...\n");
    
    alignas(max_align_t) char buf[HX_PSTRING_SIZE];
    TPascalString *pstr = (TPascalString *)buf;
    
    // Common KDX strings
    hx_pstring_from_cstr(pstr, "Public Chat");
    assert(hx_pstring_length(pstr) == 11);
    
    hx_pstring_from_cstr(pstr, "KDX");
    assert(hx_pstring_length(pstr) == 3);
    assert(hx_pstring_compare_cstr(pstr, "KDX") == 0);
    
    printf("  PASS\n");
}

int main(void) {
    printf("Running Pascal string tests...\n\n");
    
    test_basic();
    test_kdx_strings();
    
    printf("\nAll Pascal string tests passed!\n");
    return 0;
}
