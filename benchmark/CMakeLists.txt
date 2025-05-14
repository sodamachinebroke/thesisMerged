include(FetchContent)
FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.3
)
FetchContent_MakeAvailable(googlebenchmark)

function(add_compression_benchmark name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} benchmark::benchmark compression_lib)
    target_include_directories(${name} PRIVATE ${googlebenchmark_SOURCE_DIR}/include)
endfunction()

add_compression_benchmark(huffman_benchmark)
add_compression_benchmark(rle_benchmark)
add_compression_benchmark(lzw_benchmark)