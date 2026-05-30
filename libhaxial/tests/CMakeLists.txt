cmake_minimum_required(VERSION 3.15)

# Test SHA1
add_executable(test_sha1 test_sha1.c)
target_link_libraries(test_sha1 PRIVATE haxial)
add_test(NAME sha1_test COMMAND test_sha1)

# Test Blowfish
add_executable(test_blowfish test_blowfish.c)
target_link_libraries(test_blowfish PRIVATE haxial)
add_test(NAME blowfish_test COMMAND test_blowfish)

# Test errors
add_executable(test_errors test_errors.c)
target_link_libraries(test_errors PRIVATE haxial)
add_test(NAME errors_test COMMAND test_errors)
