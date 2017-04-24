//
// Created by Kieran Rimmer on 23/4/17.
//

#include "semant.hpp"

namespace RWL {

    static Symbol
            arg,
            arg2,
            Bool,
            concat,
            cool_abort,
            copy,
            Int,
            in_int,
            in_string,
            IO,
            length,
            Main,
            main_meth,
            No_class,
            No_type,
            Object,
            out_int,
            out_string,
            prim_slot,
            self,
            SELF_TYPE,
            Str,
            str_field,
            substr,
            type_name,
            val,
            True,
            False;
    //
    // Initializing the predefined symbols.
    //
    static void initialize_constants(void)
    {
        arg         = idtable.add_string("arg");
        arg2        = idtable.add_string("arg2");
        Bool        = idtable.add_string("Bool");
        concat      = idtable.add_string("concat");
        cool_abort  = idtable.add_string("abort");
        copy        = idtable.add_string("copy");
        Int         = idtable.add_string("Int");
        in_int      = idtable.add_string("in_int");
        in_string   = idtable.add_string("in_string");
        IO          = idtable.add_string("IO");
        length      = idtable.add_string("length");
        Main        = idtable.add_string("Main");
        main_meth   = idtable.add_string("main");
        out_int     = idtable.add_string("out_int");
        out_string  = idtable.add_string("out_string");
        prim_slot   = idtable.add_string("_prim_slot");
        self        = idtable.add_string("self");
        SELF_TYPE   = idtable.add_string("SELF_TYPE");
        Str         = idtable.add_string("string");
        str_field   = idtable.add_string("_str_field");
        substr      = idtable.add_string("substr");
        type_name   = idtable.add_string("type_name");
        val         = idtable.add_string("_val");
        True        = idtable.add_string("true");
        False       = idtable.add_string("false");
    }

    ostream& ExpressionTable::semant_error(Expression c)
    {
        return semant_error(c->get_filename(),c);
    }

    ostream& ExpressionTable::semant_error(Symbol filename, tree_node *t)
    {
        error_stream << filename << ":" << t->get_line_no() << ": ";
        return semant_error();
    }

    ostream& ExpressionTable::semant_error()
    {
        semant_errors++;
        return error_stream;
    }

    void function_node::publish(ExpressionTableP expTable)
    {
        SymbolTable<Symbol, Entry> params;
        params.enterscope();

        for (int i = formals->first(); formals->more(i); i = formals->next(i))
        {
            formal_node *formal = formals->nth(i);

            if (params.lookup(formal->get_name()) != NULL)
            {
//                expTable->semant_error(formal) <<
//                                                                          "Duplicated parameter name " << formal->get_name()
//                                                                          << " in method " << name << std::endl;
                return;
            }

            params.addid(formal->get_name(), formal->get_name());

            formal->publish(expTable);
        }

        params.exitscope();
    }

    ExpressionTable::ExpressionTable(Expressions exps): semant_errors(0) , error_stream(std::cerr) {
        expressions_ = exps;
    }

    void pgm::semant() {
        ExpressionTableP expTab = new ExpressionTable(exps);
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