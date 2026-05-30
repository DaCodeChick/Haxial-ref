/**
 * @file test_errors.c
 * @brief Error code tests
 */

#include <haxial.h>
#include <stdio.h>
#include <string.h>

static void test_error_strings(void) {
    const char *str;
    int pass = 1;
    
    str = hx_error_string(HX_ERROR_SUCCESS);
    if (strcmp(str, "Success") != 0) {
        printf("Error string for SUCCESS: FAIL (got '%s')\n", str);
        pass = 0;
    }
    
    str = hx_error_string(HX_ERROR_INVALID_PARAM);
    if (strcmp(str, "Invalid parameter") != 0) {
        printf("Error string for INVALID_PARAM: FAIL (got '%s')\n", str);
        pass = 0;
    }
    
    str = hx_error_string(HX_ERROR_SOCKET_CLOSED);
    if (strcmp(str, "Socket is closed") != 0) {
        printf("Error string for SOCKET_CLOSED: FAIL (got '%s')\n", str);
        pass = 0;
    }
    
    str = hx_error_string(0xDEADBEEF);
    if (strcmp(str, "Unknown error") != 0) {
        printf("Error string for unknown: FAIL (got '%s')\n", str);
        pass = 0;
    }
    
    if (pass) {
        printf("Error string test: PASS\n");
    }
}

static void test_error_helpers(void) {
    int pass = 1;
    
    if (!hx_error_success(HX_ERROR_SUCCESS)) {
        printf("hx_error_success(SUCCESS): FAIL\n");
        pass = 0;
    }
    
    if (hx_error_success(HX_ERROR_INVALID_PARAM)) {
        printf("hx_error_success(ERROR): FAIL\n");
        pass = 0;
    }
    
    if (hx_error_failed(HX_ERROR_SUCCESS)) {
        printf("hx_error_failed(SUCCESS): FAIL\n");
        pass = 0;
    }
    
    if (!hx_error_failed(HX_ERROR_OUT_OF_MEMORY)) {
        printf("hx_error_failed(ERROR): FAIL\n");
        pass = 0;
    }
    
    if (pass) {
        printf("Error helper functions test: PASS\n");
    }
}

static void test_error_codes(void) {
    /* Just verify the constants have expected values from Ghidra analysis */
    int pass = 1;
    
    if (HX_ERROR_SUCCESS != 0x00000000) {
        printf("HX_ERROR_SUCCESS value: FAIL\n");
        pass = 0;
    }
    
    if (HX_ERROR_SOCKET_CLOSED != 0x0004006e) {
        printf("HX_ERROR_SOCKET_CLOSED value: FAIL (expected 0x0004006e, got 0x%08x)\n", 
               HX_ERROR_SOCKET_CLOSED);
        pass = 0;
    }
    
    if (HX_ERROR_AUTH_FAILED != 0x00040070) {
        printf("HX_ERROR_AUTH_FAILED value: FAIL (expected 0x00040070, got 0x%08x)\n",
               HX_ERROR_AUTH_FAILED);
        pass = 0;
    }
    
    if (pass) {
        printf("Error code values test: PASS\n");
    }
}

int main(void) {
    printf("Running error code tests...\n\n");
    
    test_error_strings();
    test_error_helpers();
    test_error_codes();
    
    printf("\nError code tests complete.\n");
    return 0;
}
