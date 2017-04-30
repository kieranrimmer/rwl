
#include <assert.h>
#include "string_table.hpp"

namespace RWL {
//
// Explicit template instantiations.
// Comment out for versions of g++ prior to 2.7
//
    template
    class StringTable<IdEntry>;

    template
    class StringTable<StringEntry>;

    template
    class StringTable<IntEntry>;

    Entry::Entry(std::string s, int l, int i) : len(l), index(i) {
        str.assign(s);
    }

    int Entry::equal_string(std::string string, int length) const {
        return (len == length) && (str.compare(string) == 0);
    }

    ostream &Entry::print(ostream &s) const {
        return s << "{" << str << ", " << len << ", " << index << "}\n";
    }

    ostream &operator<<(ostream &s, const Entry &sym) {
        return s << sym.get_string();
    }


    ostream &operator<<(ostream &s, Symbol sym) {
        return s << *sym;
    }

    std::string Entry::get_string() const {
        return str;
    }

    int Entry::get_len() const {
        return len;
    }

// A Symbol is a pointer to an Entry.  Symbols are stored directly
// as nodes of the abstract syntax tree defined by the cool-tree.aps.
// The APS package requires that copy and print (called dump) functions
// be defined for components of the abstract syntax tree.
//
    Symbol copy_Symbol(const Symbol s) {
        return s;
    }

    void dump_Symbol(ostream &s, int n, Symbol sym) {
        s << sym << endl;
    }

    StringEntry::StringEntry(std::string s, int l, int i) : Entry(s, l, i) {}

    IdEntry::IdEntry(std::string s, int l, int i) : Entry(s, l, i) {}

    IntEntry::IntEntry(std::string s, int l, int i) : Entry(s, l, i) {}

    IdTable idtable;
    IntTable inttable;
    StrTable stringtable;

}
