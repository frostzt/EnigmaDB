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

if(MSVC)
    add_compile_options(/arch:AVX2)
else()
    add_compile_options(-mavx2 -msse4.2)
endif()

add_executable(lsmtree
        lib/abstract/avl.hpp
        lib/entry/entry.hpp
        lib/entry/entry.cpp
        lib/memtable/memtable.cpp
        lib/memtable/memtable.hpp
        lib/abstract/timestamp_generator.cpp
        lib/abstract/timestamp_generator.hpp
        tests/test_abs.hpp
        tests/test_main.cpp
        tests/timestamp_generator_test.hpp
        tests/timestamp_generator_test.cpp
        lib/utils/crypto_utils.cpp
        lib/utils/crypto_utils.hpp
        tests/utils/byte_utils.cpp
        tests/utils/byte_utils.hpp
        lib/utils/byte_parser.cpp
        lib/utils/byte_parser.hpp
        tests/utils/byte_parser.cpp
        tests/utils/byte_parser.hpp
        lib/utils/logger.hpp
        lib/wal/wal.cpp
        lib/wal/wal.hpp
        lib/wal/wal_codec.hpp
        tests/wal/wal_codec_test.cpp
        tests/wal/wal_codec_test.hpp
        lib/wal/wal_manager.cpp
        lib/wal/wal_manager.hpp
        tests/wal/wal_manager_test.cpp
        tests/wal/wal_manager_test.hpp
        lib/utils/constants.hpp
        lib/wal/wal_writer.cpp
        lib/wal/wal_writer.hpp
)

target_link_libraries(lsmtree PRIVATE spdlog::spdlog)
