cmake_minimum_required(VERSION 3.31.6)
project(lsmtree)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(lib)

add_executable(lsmtree
        lib/entry.hpp
        lib/entry.hpp
        lib/entry.cpp
        lib/avl.cpp
        lib/memtable.cpp
        lib/memtable.hpp
)
