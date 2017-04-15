
#ifndef _TREE_H_
#define _TREE_H_

#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include "../string_table/string_table.hpp"
#include "../rwl_util.hpp"

using Value=llvm::Value;

using namespace llvm;

namespace RWL {

    class tree_node {
    protected:
        int line_no;
    public:
    };

    class exp_node: public tree_node {
    public:

        // print function for pretty printing an expression
        virtual void print() = 0;

        // evaluation function for a leaf, replaced for interior nodes
        virtual void evaluate() = 0;

        virtual Value *codegen() = 0;
    };

    class declaration_node: public tree_node {
    public:
        Symbol type;
        Symbol name;
        exp_node *initialisation;

        declaration_node(Symbol t, Symbol n, exp_node *exp) : type(t), name(n) { initialisation = exp; }
    };

    class function_node: public tree_node {
    public:
        Symbol sym;

        void print()  { std::cout << "function node: " << sym->get_string() << std::endl; };

        void evaluate() { print(); }

    };

    class integer_node : public exp_node {

    public:
        Symbol sym;

        integer_node(Symbol value) : sym(value) {};

        void print() override;

        void evaluate() override;

        Value *codegen() override;
    };


    class operator_node : public exp_node {
    public:
        exp_node *left;
        exp_node *right;

        // the constructor for node links the node to its children,
        // and stores the character representation of the operator.
        operator_node(exp_node *L, exp_node *R) : left(L), right(R) {};
        Value *codegen() override;
    };

    class unary_minus_node : public exp_node {
    protected:
        exp_node *_exp;
    public:
        unary_minus_node(exp_node *exp) : _exp(exp) {};

        void print() override;

        void evaluate() override;

        Value *codegen() override;
    };

    class id_node : public exp_node {

    protected:

        Symbol sym;

    public:

        Symbol get_symbol() { return sym; }

        id_node(Symbol value) : sym(value) {}

        void print() override;

        void evaluate() override;

        Value *codegen() override;
    };

    class string_node : public exp_node {

    protected:

        Symbol sym;

    public:

        string_node(Symbol value): sym(value)  {};

        void print() override;

        void evaluate() override;

        Value *codegen() override;
    };

    class plus_node : public operator_node {

    public:

        // plus_node's constructor just uses node's constructor
        plus_node(exp_node *L, exp_node *R) : operator_node(L, R) {};

        void print() override;

        void evaluate() override;
        Value *codegen() override;
    };


// minus_node inherits the characteristics of node and adds its own evaluate function
    class minus_node : public operator_node {
    public:

        minus_node(exp_node *L, exp_node *R) : operator_node(L, R) {};

        void print() override;

        void evaluate() override;
        Value *codegen() override;
    };


// times_node inherits the characteristics of node and adds its own evaluate function
    class times_node : public operator_node {
    public:

        times_node(exp_node *L, exp_node *R) : operator_node(L, R) {};

        void print() override;

        void evaluate() override;
        Value *codegen() override;
    };


// divide_node inherits the characteristics of node and adds its own evaluate function
    class divide_node : public operator_node {
    public:

        divide_node(exp_node *L, exp_node *R) : operator_node(L, R) {};

        void print() override;

        void evaluate() override;
        Value *codegen() override;
    };

    class assignment_stmt : public exp_node {
    protected:
        id_node *_id;
        exp_node *exp;
    public:

        assignment_stmt(Symbol name, exp_node *expr) { _id = new id_node(name); exp = expr; };

        void print() override;

        void evaluate() override;
        virtual Value *codegen() override;
    };

    class print_stmt : public exp_node {
    protected:
        Symbol sym;
        exp_node *exp;
    public:
        print_stmt(Symbol _sym) : sym(_sym) {};
        print_stmt(exp_node *_exp) : exp(_exp) {};

        void print() override { std::cout << "Print node: " << std::endl << "\tsym:" << sym << std::endl << "\texp: " << std::endl; exp->print(); std::cout << "End print node" << std::endl;  };

        virtual void evaluate() override;
        virtual Value *codegen() override;
    };

    class pgm {
    protected:
        std::list<exp_node *> *exps;
    public:
        pgm(std::list<exp_node *> *explist) : exps(explist) {};

        void evaluate();
        Value *codegen();
    };

// the object at the base of our tree
    extern RWL::pgm *root;

}


#endif