cmake_minimum_required(VERSION 3.6)
project(rwl)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        AST/tree.cpp
        AST/tree.hpp
        string_table/string_table.cpp
        string_table/string_table.hpp
        util/util.cpp
        util/util.hpp
        lex.yy.cc
        location.hh
        main.cpp
        Makefile
        position.hh
        README.md
        rwl_driver.cpp
        rwl_driver.hpp
        rwl_lexer.l
        rwl_lexer.yy.cc
        rwl_parser.tab.cc
        rwl_parser.tab.hh
        rwl_parser.yy
        rwl_scanner.hpp
        stack.hh
        uc_copyright.h)

add_executable(rwl ${SOURCE_FILES})
add_executable(rwl_2 ${SOURCE_FILES})