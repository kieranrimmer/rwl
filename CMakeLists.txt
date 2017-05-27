cmake_minimum_required(VERSION 3.7)
project(rwl)

include_directories(/Users/kieranrimmer/packages/llvm/include /Users/kieranrimmer/packages/build/include)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp symtab.hpp list.hpp semant.cpp semant.hpp codegen_llvm/codegen_llvm.cpp codegen_llvm/codegen_llvm.hpp type_check.cpp type_check.hpp)
add_executable(rwl ${SOURCE_FILES})