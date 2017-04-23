#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "tree.hpp"

using namespace llvm;

namespace RWL {

    int node_lineno = 1;

    // IR code generation
    static LLVMContext TheContext;
    static IRBuilder<> Builder(TheContext);
    static std::unique_ptr<Module> TheModule;
    static std::map<std::string, Value *> NamedValues;

    Value *integer_node::codegen() {
        return ConstantInt::get(TheContext, APSInt(sym->get_string()));
    }

    Value *minus_node::codegen() { return nullptr; }
    Value *unary_minus_node::codegen() { return nullptr; }
    Value *print_stmt::codegen() { return nullptr; }
    Value *divide_node::codegen() { return nullptr; }
    Value *times_node::codegen() { return nullptr; }
    Value *plus_node::codegen() { return nullptr; }
    Value *id_node::codegen() { return nullptr; }
    Value *string_node::codegen() { return nullptr; }
    Value *assignment_stmt::codegen() { return nullptr; }
    Value *operator_node::codegen() { return nullptr; }
    Value *formal_node::codegen() { return nullptr; }

    tree_node::tree_node()
    {
        line_no = node_lineno;
    }

    void integer_node::print() {
        std::cout << sym->get_string();
    }

    void formal_node::print() {
        std::cout << "formal_node: type = " << type->get_string() << ", name = " << name->get_string() << std::endl;
    }

    void id_node::print() {
        std::cout << sym->get_string() << std::endl;
    }

    void string_node::print() {
        std::cout << sym->get_string() << std::endl;
    }

    void plus_node::print() {
        std::cout << "(";
        left->print();
        std::cout << " + ";
        right->print();
        std::cout << ")";
    }

    void minus_node::print() {
        std::cout << "(";
        left->print();
        std::cout << " - ";
        right->print();
        std::cout << ")";
    }

    void times_node::print() {
        std::cout << "(";
        left->print();
        std::cout << " * ";
        right->print();
        std::cout << ")";
    }


    void divide_node::print() {
        std::cout << "(";
        left->print();
        std::cout << " / ";
        right->print();
        std::cout << ")";
    }

    void unary_minus_node::print() {
        std::cout << "-"; _exp->print();

    }

    void assignment_stmt::print() {
        exp->print();
        std::cout << std::endl;
    }




    void pgm::print() {
        std::cout << "Program root node..." << exps->len() << std::endl;
        for(int i = exps->first(); exps->more(i); i = exps->next(i)) {
            Expression exp = exps->nth(i);
            std::cout << "Iterating through exp #" << i << ": " << exp << ", " << exp->len() << " " << ", line number = " << exp->get_line_no() << std::endl;
            (exps->nth(i))->print();

        }
    }


    Expressions nil_Expressions()
    {
        return new nil_node<Expression>();
    }

    Expressions single_Expressions(Expression e)
    {
        return new single_list_node<Expression>(e);
    }

    Expressions append_Expressions(Expressions p1, Expressions p2)
    {
        return new append_node<Expression>(p1, p2);
    }

    Expression block(Expressions a1) { return new block_node(a1); }

    Expression cond(Expression pred, Expression then_exp, Expression else_exp)
    {
        return new cond_node(pred, then_exp, else_exp);
    }

    Expression loop(Expression pred, Expression body)
    {
        return new loop_node(pred, body);
    }

    Formals nil_Formals()
    {
        return new nil_node<Formal>();
    }

    Formals single_Formals(Formal e)
    {
        return new single_list_node<Formal>(e);
    }

    Formals append_Formals(Formals p1, Formals p2)
    {
        return new append_node<Formal>(p1, p2);
    }

}