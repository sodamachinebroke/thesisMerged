cmake_minimum_required(VERSION 3.30)
project(thesisMerged)

set(CMAKE_CXX_STANDARD 26)

include_directories(include)

set(SOURCE_FILES
        src/lzw.cpp
        src/rle.cpp
        src/huffman.cpp
        src/compress.cpp
        main.cpp
)

add_executable(thesisMerged ${SOURCE_FILES})