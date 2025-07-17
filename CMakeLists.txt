cmake_minimum_required(VERSION 3.31.6)
project(lsmtree)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(lsmtree
        lib/avl.hpp
        lib/entry.hpp
        lib/entry.cpp
        lib/memtable.cpp
        lib/memtable.hpp
        lib/timestamp_generator.cpp
        lib/timestamp_generator.hpp
        tests/test_abs.hpp
        tests/test_main.cpp
        tests/timestamp_generator_test.hpp
        tests/timestamp_generator_test.cpp
        lib/wal.cpp
        lib/wal.hpp
        lib/utils/byte_utils.cpp
        lib/utils/byte_utils.hpp
)
