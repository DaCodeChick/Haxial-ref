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

# Test intrusive list
add_executable(test_list list_test.c)
target_link_libraries(test_list PRIVATE haxial)
add_test(NAME list_test COMMAND test_list)

# Test FNV-1a hash
add_executable(test_fnv1a fnv1a_test.c)
target_link_libraries(test_fnv1a PRIVATE haxial)
add_test(NAME fnv1a_test COMMAND test_fnv1a)

# Test Pascal strings
add_executable(test_pstring pstring_test.c)
target_link_libraries(test_pstring PRIVATE haxial)
add_test(NAME pstring_test COMMAND test_pstring)
