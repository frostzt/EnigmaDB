cmake_minimum_required(VERSION 3.31.6)
project(enigma_db)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

include(FetchContent)

# deps
FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.15.3)
FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.8.1)
FetchContent_Declare(liburing GIT_REPOSITORY https://github.com/axboe/liburing)
FetchContent_MakeAvailable(spdlog Catch2 liburing)

add_subdirectory(external/lz4/build/cmake lz4-build)

if(MSVC)
    add_compile_options(/arch:AVX2)
else()
    add_compile_options(-mavx2 -msse4.2)
endif()

# libs
add_library(enigma_core
        lib/abstract/avl.hpp
        lib/entry/entry.hpp
        lib/entry/entry.cpp
        lib/memtable/memtable.cpp
        lib/memtable/memtable.hpp
        lib/abstract/timestamp_generator.cpp
        lib/abstract/timestamp_generator.hpp
        lib/utils/crypto_utils.cpp
        lib/utils/crypto_utils.hpp
        lib/utils/byte_parser.cpp
        lib/utils/byte_parser.hpp
        lib/utils/logger.hpp
        lib/wal/wal.cpp
        lib/wal/wal.hpp
        lib/wal/wal_codec.hpp
        lib/wal/wal_manager.cpp
        lib/wal/wal_manager.hpp
        lib/utils/constants.hpp
        lib/wal/wal_writer.cpp
        lib/wal/wal_writer.hpp
        lib/entry/key.cpp
        lib/entry/key.hpp
        lib/entry/core_constants.hpp
        lib/datatypes/field.hpp
        lib/datatypes/field.cpp
        lib/datatypes/type_descriptor.hpp
        lib/datatypes/comparator.hpp
        lib/datatypes/types/uuid.hpp
        lib/datatypes/serializer.hpp
        lib/datatypes/default_descriptor.hpp
        lib/datatypes/type_registry.cpp
        lib/datatypes/type_registry.hpp
        lib/datatypes/field_type.hpp
        lib/datatypes/field_value.hpp
        lib/memtable/memtable_manager.cpp
        lib/memtable/memtable_manager.hpp
        lib/compression/compressor.hpp
        lib/sstable/key_encoder.cpp
        lib/sstable/key_encoder.hpp
        lib/sstable/block_encoder.cpp
        lib/sstable/block_encoder.hpp
        lib/sstable/filter_policy.cpp
        lib/sstable/filter_policy.hpp
        lib/compression/lz_4_compressor.cpp
        lib/compression/lz_4_compressor.hpp
        lib/compression/noop_compressor.hpp
        lib/utils/vint/vint.hpp
)

target_include_directories(enigma_core PUBLIC .)
target_link_libraries(enigma_core PUBLIC spdlog::spdlog lz4)

# Executables
## Core DB
option(ENIGMA_BUILD_APP "Build the enigma binary" OFF)
if(ENIGMA_BUILD_APP)
    add_executable(enigma_db
            lib/main.cpp
    )
    target_link_libraries(enigma_db PRIVATE enigma_core)
endif()

## Tests
add_executable(tests
        # Timestamps
        tests/timestamp_generator_test.cpp

        # Datatypes
        tests/datatypes/test_field_serialization.cpp
        tests/datatypes/types/test_uuid_type.cpp

        # Compression
        tests/compression/test_noop_compression.cpp
        tests/compression/test_lz4_compression.cpp

        # WAL
        tests/wal/test_writer_behavior.cpp
        tests/wal/test_wal_codec_encode_decode.cpp
        tests/wal/test_wal_manager.cpp

        # MemTable
        tests/memtable/test_memtable_put_get.cpp

        # Utils
        tests/utils/vint/test_varint_encode.cpp
        tests/utils/test_byte_parser.cpp
        tests/utils/test_byte_utils_entry.cpp
        tests/sandbox.cpp
)

target_link_libraries(tests PRIVATE enigma_core Catch2::Catch2WithMain)
