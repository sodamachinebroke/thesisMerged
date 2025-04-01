# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.0
)
FetchContent_MakeAvailable(googletest)

# Test targets
function(add_compression_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE
            compression_lib
            gtest_main
    )
    add_test(NAME ${name} COMMAND ${name})
endfunction()

# Register all tests
add_compression_test(huffman_tests)
add_compression_test(rle_tests)
add_compression_test(lzw_tests)
# Add more as needed...