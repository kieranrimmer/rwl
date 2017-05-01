//
// Created by Kieran Rimmer on 23/4/17.
//

#include "semant.hpp"

namespace RWL {

    extern int node_lineno;

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
            False,
            NoType;

    //
    // Initializing the predefined symbols.
    //
    static void initialize_constants(void) {
        arg = idtable.add_string("arg");
        arg2 = idtable.add_string("arg2");
        Bool = idtable.add_string("Bool");
        concat = idtable.add_string("concat");
        cool_abort = idtable.add_string("abort");
        copy = idtable.add_string("copy");
        Int = idtable.add_string("int");
        in_int = idtable.add_string("in_int");
        in_string = idtable.add_string("in_string");
        IO = idtable.add_string("IO");
        length = idtable.add_string("length");
        Main = idtable.add_string("Main");
        main_meth = idtable.add_string("main");
        out_int = idtable.add_string("out_int");
        out_string = idtable.add_string("out_string");
        prim_slot = idtable.add_string("_prim_slot");
        self = idtable.add_string("self");
        SELF_TYPE = idtable.add_string("SELF_TYPE");
        Str = idtable.add_string("string");
        str_field = idtable.add_string("_str_field");
        substr = idtable.add_string("substr");
        type_name = idtable.add_string("type_name");
        val = idtable.add_string("_val");
        True = idtable.add_string("true");
        False = idtable.add_string("false");
        NoType = idtable.add_string("_no_type");

    }

    ostream &ExpressionTable::semant_error(Expression c) {
        return semant_error(c->get_filename(), c);
    }

    ostream &ExpressionTable::semant_error(Symbol filename, tree_node *t) {
        error_stream << red << "Semantic error: " << norm << filename << ":" << t->get_line_no() << ": ";
        return semant_error();
    }

    ostream &ExpressionTable::semant_error() {
        semant_errors++;
        return error_stream;
    }

    function_node *ExpressionTable::get_function(Symbol s) {
        function_node *method = dynamic_cast<function_node *>(functions_.lookup(s));
        std::cout << "get_function() caled!!!" << std::endl;

        if (method == NULL)
            semant_error() << "Method not found: " << s->get_string() << std::endl;

        else if (method->name == s) {
            std::cout << "Method found: " << s->get_string() << std::endl;
            return method;

        }
        return method;
    }

    void function_node::publish(ExpressionTableP expTable) {
        expTable->functions_.addid(this->name, this);
        SymbolTable<Symbol, Entry> params;
        params.enterscope();

        for (int i = formals->first(); formals->more(i); i = formals->next(i)) {
            formal_node *formal = formals->nth(i);

            if (params.lookup(formal->get_name()) != NULL) {
                expTable->semant_error(formal) <<
                                               "Duplicated parameter name " << formal->get_name()
                                               << " in method " << name << std::endl;
                return;
            }

            params.addid(formal->get_name(), formal->get_name());

            formal->publish(expTable);
        }

        params.exitscope();
    }

    void formal_node::publish(ExpressionTableP expTable) {
        if (name == self) {
            expTable->semant_error() <<
                                     "Using keyword as parameter name: " << name
                                     << std::endl;
            return;
        }
        expTable->symbols_.addid(name, type);
    }

    ExpressionTable::ExpressionTable(Expressions exps) : semant_errors(0), error_stream(std::cerr) {
        expressions_ = exps;
        symbols_.enterscope();
        functions_.enterscope();
    }

    void pgm::semant() {
        initialize_constants();
        ExpressionTableP expTab = new ExpressionTable(exps);
        exps->semant(expTab);
    }

    Symbol block_node::semant(ExpressionTableP expTab) {
        return body->semant(expTab);
    }

    Symbol declaration_node::semant(ExpressionTableP expTab) {
        expTab->symbols_.addid(this->name, this->type);
        return initialisation->semant(expTab);
    }

    Symbol loop_node::semant(ExpressionTableP expTab) {
        return NoType;
    }

    Symbol cond_node::semant(ExpressionTableP expTab) {
        return NoType;
    }

    Symbol function_node::semant(ExpressionTableP expTab) {
        std::cout << blue << "entered function_node semant() function" << norm << std::endl;
        publish(expTab);
        return NoType;
    }

    Symbol dispatch_node::semant(ExpressionTableP expTab) {
        function_node *f = expTab->get_function(name);
        std::cout << "dispatch_node::semant()" << std::endl;
        std::cout << "dispatch_node::semant() -- function_node found: " << f->name->get_string() << std::endl;
        return f->body->semant(expTab);
    }

    Symbol integer_node::semant(ExpressionTableP expTab) {
        set_type(Int);
        return Int;
    }

    Symbol formal_node::semant(ExpressionTableP expTab) {
        return type;
    }

    Symbol operator_node::semant(ExpressionTableP expTab) {
        return NoType;
    }

    Symbol unary_minus_node::semant(ExpressionTableP expTab) {
        set_type(Int);
        return Int;
    }

    Symbol id_node::semant(ExpressionTableP expTab) {
        return True;
    }

    Symbol string_node::semant(ExpressionTableP expTab) {
        set_type(Str);
        return Str;
    }

    Symbol plus_node::semant(ExpressionTableP expTab) {
        Symbol type1 = left->semant(expTab);
        Symbol type2 = right->semant(expTab);
        if (type1 != Int || type2 != Int) {
            expTab->semant_error(this) << "Addition used with non Integer variable" << std::endl;
            set_type(NoType);
            return NoType;
        }
        set_type(Int);
        return Int;
    }

    Symbol minus_node::semant(ExpressionTableP expTab) {
        Symbol type1 = left->semant(expTab);
        Symbol type2 = right->semant(expTab);
        if (type1 != Int || type2 != Int) {
            expTab->semant_error(this) << "Subtraction used with non Integer variable" << std::endl;
            set_type(NoType);
            return NoType;
        }
        set_type(Int);
        return Int;
    }

    Symbol times_node::semant(ExpressionTableP expTab) {
        Symbol type1 = left->semant(expTab);
        Symbol type2 = right->semant(expTab);
        if (type1 != Int || type2 != Int) {
            expTab->semant_error(this) << "Multiplication used with non Integer variable" << std::endl;
            set_type(NoType);
            return NoType;
        }
        set_type(Int);
        return Int;
    }

    Symbol divide_node::semant(ExpressionTableP expTab) {
        Symbol type1 = left->semant(expTab);
        Symbol type2 = right->semant(expTab);
        if (type1 != Int || type2 != Int) {
            expTab->semant_error(this) << "Division used with non Integer variable" << std::endl;
            set_type(NoType);
            return NoType;
        }
        set_type(Int);
        return Int;
    }

    Symbol assignment_stmt::semant(ExpressionTableP expTab) {
        Symbol type = expTab->symbols_.lookup(_id->get_symbol());
        if (type == nullptr) {
            expTab->semant_error(this) <<
               "Identifier not declared: " << _id->get_string() << std::endl;
            set_type(NoType);
            return NoType;
        }

        Symbol type2 = exp->semant(expTab);

        if (type != type2) {
            expTab->semant_error(this) <<
               "Wrong type in assign statement: " << type << " != " << type2  << std::endl;
            set_type(NoType);
            return NoType;
        }

        set_type(type);
        return type;
    }

    Symbol print_stmt::semant(ExpressionTableP expTab) {
        return NoType;
    }

}