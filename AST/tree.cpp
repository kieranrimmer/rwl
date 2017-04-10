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

    Value *number_node::codegen() {
        return ConstantInt::get(TheContext, APSInt(Val));
    }

    // end IR code generation

    // the constructor for node links the node to its children,
    // and stores the character representation of the operator.
//    operator_node::operator_node(exp_node<int> *L, exp_node<int> *R) {
//        left = L;
//        right = R;
//    }


//    number_node::number_node(int value) {
//        Val = value;
//    }

    void number_node::print() {
        std::cout << Val;
    }

    int number_node::evaluate() {
        std::cout << "number_node: operand = " << Val << std::endl;
        return Val;
    }

    std::string string_node::evaluate() {
        std::cout << "string_node: operand = " << Val << std::endl;
        return Val;
    }

    Value *integer_node::codegen() {
        return ConstantInt::get(TheContext, APSInt(Val));
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


    integer_node::integer_node(char *value) {
        std::cout << "integer_node: value = " << value << std::endl;
        Val = std::stoi(value);
    }

    integer_node::integer_node(Symbol value) : sym(value) {
        std::cout << "integer_node constructor: value = " << value->get_string() << std::endl;
        Val = std::stoi(value->get_string());
    }

    void integer_node::print() {
        std::cout << Val;
    }

    int integer_node::evaluate() {
        std::cout << "integer_node: operand = " << sym->get_string() << std::endl;
        return Val;
    }

//    id_node::id_node(std::string value) : id(value) {}



    void id_node::print() {
        std::cout << Val;
    }

    std::string id_node::evaluate() {
        std::cout << "id_node: " << Val << " = " << idTable[Val] << std::endl;
        return Val;
    }

//    string_node::string_node(std::string value) : Val(value) {}

//    string_node::string_node(Symbol value) : Val((std::string) value->get_string()) {}

    void string_node::print() {
        std::cout << Val;
    }

//    int string_node::evaluate() {
//        std::cout << "id_node: " << Val << " = " << stringtable.lookup_std_string(Val) << std::endl;
//        return stringtable.lookup_std_string(Val)->get_len() > 0;
//    }

// plus_node inherits the characteristics of node and adds its own evaluate function
    // plus_node's constructor just uses node's constructor
//    plus_node::plus_node(exp_node *L, exp_node *R) : operator_node(L, R) {
//    }

    void plus_node::print() {
        std::cout << "(";
        left->print();
        std::cout << " + ";
        right->print();
        std::cout << ")";
    }

    int plus_node::evaluate() {
        int left_num, right_num;

        left_num = left->evaluate();
        right_num = right->evaluate();

        Val = left_num + right_num;
        std::cout << "plus_node: " << left_num << " + " << right_num << " = " << Val << "\n";
        return Val;
    }


// minus_node inherits the characteristics of node and adds its own evaluate function
//    minus_node::minus_node(exp_node *L, exp_node *R) : operator_node(L, R) {
//    }

    void minus_node::print() {
        std::cout << "(";
        left->print();
        std::cout << " - ";
        right->print();
        std::cout << ")";
    }

    int minus_node::evaluate() {
        int left_num, right_num;

        left_num = left->evaluate();
        right_num = right->evaluate();

        Val = left_num - right_num;
        std::cout << "minus_node: " << left_num << " - " << right_num << " = " << Val << "\n";
        return Val;
    }


// times_node inherits the characteristics of node and adds its own evaluate function
//    times_node::times_node(exp_node *L, exp_node *R) : operator_node(L, R) {
//    }

    void times_node::print() {
        std::cout << "(";
        left->print();
        std::cout << " * ";
        right->print();
        std::cout << ")";
    }

    int times_node::evaluate() {
        int left_num, right_num;

        left_num = left->evaluate();
        right_num = right->evaluate();

        Val = left_num * right_num;
        std::cout << "times_node: " << left_num << " * " << right_num << " = " << Val << "\n";
        return Val;
    }


// divide_node inherits the characteristics of node and adds its own evaluate function

//    divide_node::divide_node(exp_node *L, exp_node *R) : operator_node(L, R) {
//    }

    void divide_node::print() {
        std::cout << "(";
        left->print();
        std::cout << " / ";
        right->print();
        std::cout << ")";
    }

    int divide_node::evaluate() {
        int left_num, right_num;

        left_num = left->evaluate();
        right_num = right->evaluate();

        if (right_num) {
            Val = (float) left_num / (float) right_num;
            std::cout << "divide_node: " << left_num << " / " << right_num << " = " << Val << "\n";
            return Val;
        } else {
            std::cout << "divide_node: division by zero -> " << left_num << " / " << 0 << std::endl;
            // you have to include stdlib.h for exit
            exit(1);
        }
    }

// unary_minus_node inherits the characteristics of node and adds its own evaluate function
//    unary_minus_node::unary_minus_node(exp_node *L) : exp(L) {}

    void unary_minus_node::print() {
        std::cout << "-" << exp;

    }

    int unary_minus_node::evaluate() {
        int expValue = _exp->evaluate();
        Val = -expValue;

        std::cout << "unary_minus_node: " << "-\t" << expValue << " = " << Val << "\n";
        return Val;
    }

//    assignment_stmt::assignment_stmt(std::string name, exp_node<int> *expression)
//            : id(name), exp(expression) {}

//    assignment_stmt::assignment_stmt(Symbol symbol, exp_node<int> *expression)
//            : id((std::string) symbol->get_string()), exp(expression) {}

    void assignment_stmt::print() {
        std::cout << id.evaluate() << " = ";
        exp->print();
        std::cout << std::endl;
    }

    void assignment_stmt::evaluate() {
        int result = exp->evaluate();
        std::cout << "assignment_node: " << id.evaluate() << " = " << result << std::endl << std::endl;
        idTable[id.evaluate()] = result;
    }

    print_stmt::print_stmt(std::string name) : id(name) {}

    print_stmt::print_stmt(int num) : id(std::to_string(num)) {}

    void print_stmt::evaluate() {
        std::cout << "print_node: " << id << " = " << idTable[id] << std::endl << std::endl;
    }

//    pgm::pgm(std::list < statement * > *stmtList) : stmts(stmtList) {}

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