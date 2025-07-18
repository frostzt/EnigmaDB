cmake_minimum_required(VERSION 3.31.6)
project(lsmtree)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

include(FetchContent)

# External content
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.3
)

FetchContent_MakeAvailable(spdlog)

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
        lib/utils/crypto_utils.cpp
        lib/utils/crypto_utils.hpp
        tests/utils/byte_utils.cpp
        tests/utils/byte_utils.hpp
        lib/utils/byte_parser.cpp
        lib/utils/byte_parser.hpp
        tests/utils/byte_parser.cpp
        tests/utils/byte_parser.hpp
        lib/utils/logger.hpp
)

target_link_libraries(lsmtree PRIVATE spdlog::spdlog)
