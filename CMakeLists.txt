cmake_minimum_required(VERSION 3.30)
project(thesisMerged)

set(CMAKE_CXX_STANDARD 26)

# Create a library from your sources
add_library(compression_lib
        src/lzw.cpp
        src/rle.cpp
        src/huffman.cpp
        src/compress.cpp
)

# Public headers (exposed to tests and main executable)
target_include_directories(compression_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Main executable
add_executable(thesisMerged main.cpp)
target_link_libraries(thesisMerged PRIVATE compression_lib)

# Enable testing
enable_testing()
add_subdirectory(test)