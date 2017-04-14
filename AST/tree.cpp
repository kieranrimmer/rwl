#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "tree.hpp"

using namespace llvm;

namespace RWL {


    // IR code generation
    static LLVMContext TheContext;
    static IRBuilder<> Builder(TheContext);
    static std::unique_ptr<Module> TheModule;
    static std::map<std::string, Value *> NamedValues;

//    Value *number_node::codegen() {
//        return ConstantInt::get(TheContext, APSInt(Val));
//    }


    void string_node::evaluate() {
        std::cout << "string_node: operand = " << sym->get_string() << std::endl;
    }

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

    void integer_node::print() {
        std::cout << sym->get_string();
    }

    void integer_node::evaluate() {
        std::cout << "integer_node: operand = " << sym->get_string() << std::endl;
    }

    void id_node::print() {
        std::cout << sym->get_string();
    }

    void id_node::evaluate() {
        std::cout << "id_node: " << idTable[sym->get_string()] << std::endl;
    }

    void string_node::print() {
        std::cout << sym->get_string();
    }

    void plus_node::print() {
        std::cout << "(";
        left->print();
        std::cout << " + ";
        right->print();
        std::cout << ")";
    }

    void plus_node::evaluate() {

        std::cout << "plus_node: ";
        left->print();
        std::cout << " ";
        right->print();
        std::cout << std::endl;
    }

    void minus_node::print() {
        std::cout << "(";
        left->print();
        std::cout << " - ";
        right->print();
        std::cout << ")";
    }

    void minus_node::evaluate() {
        std::cout << "minus_node: ";
        left->print();
        std::cout  << " ";
        right->print();
    }

    void times_node::print() {
        std::cout << "(";
        left->print();
        std::cout << " * ";
        right->print();
        std::cout << ")";
    }

    void times_node::evaluate() {
        std::cout << "times_node: "; left->print(); std::cout << " "; right->print();
    }

    void divide_node::print() {
        std::cout << "(";
        left->print();
        std::cout << " / ";
        right->print();
        std::cout << ")";
    }

    void divide_node::evaluate() {
        std::cout << "divide_node: "; left->print(); std::cout << " "; right->print();
    }

    void unary_minus_node::print() {
        std::cout << "-"; _exp->print();

    }

    void unary_minus_node::evaluate() {
        std::cout << "unary_minus_node: " << "-\t";
        _exp->print();
    }

    void assignment_stmt::print() {
        exp->print();
        std::cout << std::endl;
    }

    void assignment_stmt::evaluate() {
        std::cout << "assignment_node: " << _id->get_symbol()->get_string() << " = ";
        std::cout << "assignment_node: " << _id->get_symbol()->get_string() << " = ";
        exp->print();
    }



    void print_stmt::evaluate() {
        std::cout << "print_node: " << sym->get_string() << " = " << idTable[sym->get_string()] << std::endl;
    }

    void pgm::evaluate() {
        std::list<statement *>::iterator stmtIter;
        std::cout << "Program root node..." << std::endl;
        for (stmtIter = stmts->begin(); stmtIter != stmts->end();
             stmtIter++) {
            (*stmtIter)->print();
            (*stmtIter)->evaluate();
        }
    }

    std::map<std::string, float> idTable;

}