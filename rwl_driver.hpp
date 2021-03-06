#ifndef __RWLDRIVER_HPP__
#define __RWLDRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>

#include "rwl_scanner.hpp"
#include "AST/tree.hpp"
#include "rwl_parser.tab.hh"
#include "string_table/string_table.hpp"
#include "util/util.hpp"

namespace RWL {

    class RWL_Driver {
    public:
        RWL_Driver() = default;

        virtual ~RWL_Driver();

        /**
         * parse - parse from a file
         * @param filename - valid string with input file
         */
        void parse(const char *const filename);

        /**
         * parse - parse from a c++ input stream
         * @param is - std::istream&, valid input stream
         */
        void parse(std::istream &iss);

        void add_upper();

        void add_lower();

        void add_word(const std::string &word);

        void add_newline();

        void add_char();

        void semant();

        void codegen();

        std::ostream &print(std::ostream &stream);

        std::ostream &declare_start(std::ostream &stream);

    private:

        void parse_helper(std::istream &stream);

        std::size_t chars = 0;
        std::size_t words = 0;
        std::size_t lines = 0;
        std::size_t uppercase = 0;
        std::size_t lowercase = 0;
        RWL::RWL_Parser *parser = nullptr;
        RWL::RWL_Scanner *scanner = nullptr;


    };



}
#endif /* END __RWLDRIVER_HPP__ */
