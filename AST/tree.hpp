
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

    typedef exp_node *Expression;



    template <class Elem> class list_node: public exp_node {
    public:
        std::list<Elem> *elements;

        list_node(std::list<Elem> *explist) : elements(explist) {}

        list_node() {}



        void print() override {
            evaluate();
        }

        void evaluate() override {
            typename std::list<Elem>::iterator expIter;
            std::cout << red << "templated list node (block), with " << elements->size() << " formal parameters" << norm << std::endl;
            for (expIter = elements->begin(); expIter != elements->end();
                 expIter++) {
                std::cout << blue << "@ element #" << (*expIter) << norm << std::endl;
                (*expIter)->print();
                (*expIter)->evaluate();
            }
        }

        Value *codegen() override { return nullptr; }

    };

    typedef list_node<Expression> Expressions_class;
    typedef Expressions_class *Expressions;

    class declaration_node: public exp_node {
    public:
        Symbol type;
        Symbol name;
        exp_node *initialisation;

        void print() override  { std::cout << "declaration node: type: " << type->get_string() << ", variable name: " << name->get_string() << ", intitialisation: "; initialisation->print(); std::cout << std::endl; }

        void evaluate() override { print(); }

        Value *codegen() override { return nullptr; }

        declaration_node(Symbol t, Symbol n, exp_node *exp) : type(t), name(n) { initialisation = exp; }
    };

    class function_node: public exp_node {
    public:
        Symbol returnType;
        Symbol name;
        std::list<RWL::exp_node *> *formals;
        exp_node *body;

        void print() override  {
            std::cout << "function node: return type: " << returnType->get_string() << ", function name: " << name->get_string() << norm << std::endl;
            std::list<Expression>::iterator expIter;
            for (expIter = formals->begin(); expIter != formals->end();
                 expIter++) {
                std::cout << blue << "@ formal parameter = " << norm;
                (*expIter)->print();
                std::cout << std::endl;
            }
            std::cout << ", function body: ";
            body->print(); std::cout << std::endl;
        }

        void evaluate() override { print(); }

        Value *codegen() override { return nullptr; }

        function_node(Symbol t, Symbol n, std::list<RWL::exp_node *> *formal_list, exp_node *exp) : returnType(t), name(n) { formals = formal_list; body = exp; }

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