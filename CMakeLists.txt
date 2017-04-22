cmake_minimum_required(VERSION 3.7)
project(rwl)

include_directories(/Users/kieranrimmer/packages/llvm/include /Users/kieranrimmer/packages/build/include)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp symtab.hpp list.hpp)
add_executable(rwl ${SOURCE_FILES})