
#include "../string_table/string_table.hpp"   // Symbol <-> String conversions
#include "util.hpp"


// #define CHECK_TABLES

// sm: fixed an off-by-one error here; code assumed there were 80 spaces, but
// in fact only 79 spaces were there; I've made it 80 now
//                                1         2         3         4         5         6         7
//                      01234567890123456789012345678901234567890123456789012345678901234567890123456789
static char padding[] = "                                                                                ";      // 80 spaces for padding


std::string pad(int n) {
    if (n > 80) return (std::string) padding;
    if (n <= 0)  return (std::string) "";
    return padding+(80-n);
}
