//
// Created by Kieran Rimmer on 23/4/17.
//

#include "semant.hpp"

namespace RWL {

    void pgm::semant() {
        exps->semant(exps);
    }

    void block_node::semant(Expressions exprs) {}

    void declaration_node::semant(Expressions exprs) {}

    void loop_node::semant(Expressions exprs) {}

    void cond_node::semant(Expressions exprs) {}

    void function_node::semant(Expressions exprs) {
        std::cout << blue << "entered function_node semant() function" << std::endl;
    }

    void dispatch_node::semant(Expressions exprs) {}

    void integer_node::semant(Expressions exprs) {}

    void formal_node::semant(Expressions exprs) {}

    void operator_node::semant(Expressions exprs) {}

    void unary_minus_node::semant(Expressions exprs) {}

    void id_node::semant(Expressions exprs) {}

    void string_node::semant(Expressions exprs) {}

    void plus_node::semant(Expressions exprs) {}

    void minus_node::semant(Expressions exprs) {}

    void times_node::semant(Expressions exprs) {}

    void divide_node::semant(Expressions exprs) {}

    void assignment_stmt::semant(Expressions exprs) {}

    void print_stmt::semant(Expressions exprs) {}

}