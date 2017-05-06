//
// Created by Kieran Rimmer on 23/4/17.
//

#ifndef RWL_SEMANT_HPP
#define RWL_SEMANT_HPP

#include "AST/tree.hpp"
#include "symtab.hpp"
#include <iostream>

namespace RWL {

    class ExpressionTable {
    private:
        int semant_errors;
        void install_basic_classes();
        ostream& error_stream;
        Expressions expressions_;
        Expression current_expression_;

    public:
        ExpressionTable(Expressions);
        int errors() { return semant_errors; }
        ostream& semant_error();
        ostream& semant_error(Expression exp);
        ostream& semant_error(Symbol filename, tree_node *t);

        Expression get_expression(Symbol name);
        bool check_expression(Expression c);

        void set_current_expression(Expression exp) { current_expression_ = exp; }
        Expression get_current_expression() { return current_expression_; }

        Symbol get_return_type(Symbol m);
        function_node* get_function(Symbol m);

        void publish_variables(function_node* m);


        SymbolTable<Symbol, Entry> symbols_;
        SymbolTable<Symbol, function_node> functions_;

    };
}


#endif //RWL_SEMANT_HPP
