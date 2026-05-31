/**
 * @file errno_convert_test.c
 * @brief Test errno to KDX error code conversion
 */

#include "haxial/errors.h"
#include <errno.h>
#include <stdio.h>
#include <assert.h>

#define TEST(name) static void test_##name(void)
#define RUN_TEST(name) do { \
    printf("Running test_%s...\n", #name); \
    test_##name(); \
    printf("  PASS\n"); \
} while(0)

/* Test permission errors */
TEST(permission_errors) {
    assert(hx_errno_to_error(EPERM) == HX_ERROR_PERMISSION_DENIED);
    assert(hx_errno_to_error(EACCES) == HX_ERROR_PERMISSION_DENIED);
}

/* Test file errors */
TEST(file_errors) {
    assert(hx_errno_to_error(ENOENT) == HX_ERROR_FILE_NOT_FOUND);
    assert(hx_errno_to_error(EEXIST) == HX_ERROR_FILE_EXISTS);
    assert(hx_errno_to_error(ENOTDIR) == HX_ERROR_NOT_DIRECTORY);
    assert(hx_errno_to_error(EISDIR) == HX_ERROR_IS_DIRECTORY);
    assert(hx_errno_to_error(EROFS) == HX_ERROR_READ_ONLY_FS);
    assert(hx_errno_to_error(ENAMETOOLONG) == HX_ERROR_NAME_TOO_LONG);
}

/* Test memory errors */
TEST(memory_errors) {
    assert(hx_errno_to_error(ENOMEM) == HX_ERROR_OUT_OF_MEMORY);
    assert(hx_errno_to_error(EFAULT) == HX_ERROR_BAD_ADDRESS);
}

/* Test parameter errors */
TEST(parameter_errors) {
    assert(hx_errno_to_error(EINVAL) == HX_ERROR_INVALID_PARAM);
    assert(hx_errno_to_error(E2BIG) == HX_ERROR_ARGUMENT_TOO_LONG);
}

/* Test network unreachable (many-to-one mapping) */
TEST(network_unreachable) {
    TErrorCode expected = HX_ERROR_NETWORK_UNREACHABLE;
    
    assert(hx_errno_to_error(ENETDOWN) == expected);
    assert(hx_errno_to_error(ENETUNREACH) == expected);
    assert(hx_errno_to_error(EHOSTUNREACH) == expected);
}

/* Test connection aborted (many-to-one mapping) */
TEST(connection_aborted) {
    TErrorCode expected = HX_ERROR_CONNECTION_ABORTED;
    
    assert(hx_errno_to_error(ENETRESET) == expected);
    assert(hx_errno_to_error(ECONNABORTED) == expected);
    assert(hx_errno_to_error(EHOSTDOWN) == expected);
}

/* Test address errors (many-to-one mapping) */
TEST(address_errors) {
    TErrorCode expected = HX_ERROR_ADDRESS_IN_USE;
    
    assert(hx_errno_to_error(EADDRINUSE) == expected);
    assert(hx_errno_to_error(EADDRNOTAVAIL) == expected);
}

/* Test connection errors */
TEST(connection_errors) {
    assert(hx_errno_to_error(ECONNRESET) == HX_ERROR_CONNECTION_RESET);
    assert(hx_errno_to_error(ECONNREFUSED) == HX_ERROR_CONNECTION_REFUSED);
    assert(hx_errno_to_error(ETIMEDOUT) == HX_ERROR_CONNECTION_TIMEOUT);
}

/* Test resource errors */
TEST(resource_errors) {
    assert(hx_errno_to_error(EBUSY) == HX_ERROR_RESOURCE_BUSY);
    assert(hx_errno_to_error(ESRCH) == HX_ERROR_NO_SUCH_PROCESS);
}

/* Test unknown error (default case) */
TEST(unknown_error) {
    /* Use an errno value that's not mapped */
    assert(hx_errno_to_error(999) == HX_ERROR_UNKNOWN);
}

/* Test all many-to-one mappings produce same result */
TEST(many_to_one_consistency) {
    /* Verify that related errors map to same code */
    assert(hx_errno_to_error(EPERM) == hx_errno_to_error(EACCES));
    assert(hx_errno_to_error(ENETDOWN) == hx_errno_to_error(ENETUNREACH));
    assert(hx_errno_to_error(ENETDOWN) == hx_errno_to_error(EHOSTUNREACH));
    assert(hx_errno_to_error(ENETRESET) == hx_errno_to_error(ECONNABORTED));
    assert(hx_errno_to_error(ENETRESET) == hx_errno_to_error(EHOSTDOWN));
    assert(hx_errno_to_error(EADDRINUSE) == hx_errno_to_error(EADDRNOTAVAIL));
}

int main(void) {
    printf("Testing errno to KDX error conversion...\n\n");
    
    RUN_TEST(permission_errors);
    RUN_TEST(file_errors);
    RUN_TEST(memory_errors);
    RUN_TEST(parameter_errors);
    RUN_TEST(network_unreachable);
    RUN_TEST(connection_aborted);
    RUN_TEST(address_errors);
    RUN_TEST(connection_errors);
    RUN_TEST(resource_errors);
    RUN_TEST(unknown_error);
    RUN_TEST(many_to_one_consistency);
    
    printf("\nAll errno conversion tests passed!\n");
    return 0;
}
