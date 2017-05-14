
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
#include "llvm/IR/TypeBuilder.h"

#include "../string_table/string_table.hpp"
#include "../util/util.hpp"
#include "../symtab.hpp"

using Value=llvm::Value;

using namespace llvm;



namespace RWL {

    class ExpressionCodeTable;
    typedef ExpressionCodeTable *ExpressionCodeTableP;

    class ExpressionTable;
    typedef ExpressionTable *ExpressionTableP;

    extern int node_lineno;
    extern Symbol filename_symbol;

    class tree_node;
    class exp_node;
    class formal_node;
    template <class Elem> class list_node;
    template <class Elem> class nil_node;
    template <class Elem> class single_list_node;
    template <class Elem> class append_node;

    typedef exp_node *Expression;
    typedef list_node<Expression> Expressions_class;
    typedef Expressions_class *Expressions;

    typedef formal_node *Formal;
    typedef list_node<Formal> Formals_class;
    typedef Formals_class *Formals;

    extern std::string pad(int n);

    extern Symbol filename_symbol;

    class pgm {
    protected:
        Expressions exps;
        Symbol filename;
    public:
        pgm(Expressions explist) { exps = explist; filename = filename_symbol; }

        void codegen();

        void print();

        void semant();

        Symbol get_filename() { return filename; }

    };

    // the object at the base of our tree
    extern pgm *root;

    class tree_node {
    protected:
        int line_no;
    public:
        tree_node();

        virtual tree_node *copy() = 0;

        virtual void print() = 0;

        virtual Value *codegen(ExpressionCodeTableP) = 0;

        virtual int len() { return 0; }



        int get_line_no() { return line_no; }

        virtual Symbol get_filename() { return root->get_filename(); }

    };

    class exp_node: public tree_node {
    public:
        Symbol type;
        Symbol get_type() { return type; }
        exp_node *set_type(Symbol s) { type = s; return this; }
        virtual Symbol semant(ExpressionTableP xps) = 0;

    };

    template <class Elem> class list_node: public exp_node {
    public:

        list_node() {}

        tree_node *copy() override          { return copy_list();  }
        Elem nth(int n);
        //
        // The next three define a simple iterator.
        //
        int first()      { return 0; }
        int next(int n)  { return n + 1; }
        int more(int n)  { return (n < len()); }

        virtual list_node<Elem> *copy_list() = 0;
        virtual ~list_node() { }
        int len() override  { return 0; }

        virtual Elem nth_length(int n, int &len) = 0;

        static list_node<Elem> *nil();
        static list_node<Elem> *single(Elem);
        static list_node<Elem> *append(list_node<Elem> *l1,list_node<Elem> *l2);

        void print() override {
            std::cout << red << "templated list node (block), with  formal parameters" << norm << std::endl;
        }


    };





    template <class Elem> class nil_node : public list_node<Elem> {
    public:
        list_node<Elem> *copy_list() override;
        int len() override;
        Elem nth_length(int n, int &len) override;
        void print() override { std::cout <<  "nil node" << std::endl; }
        Symbol semant(ExpressionTableP exprs) override;
        Value *codegen(ExpressionCodeTableP) override;
    };

    template <class Elem> class single_list_node : public list_node<Elem> {
        Elem elem;
    public:
        single_list_node(Elem t) {
            elem = t;
        }
        list_node<Elem> *copy_list() override;
        int len() override;
        Elem nth_length(int n, int &len) override;
        void print() override { std::cout <<  "single list node: " << std::endl; elem->print(); }
        Symbol semant(ExpressionTableP exprs) override;
        Value *codegen(ExpressionCodeTableP) override;
    };


    template <class Elem> class append_node : public list_node<Elem> {
    private:
        list_node<Elem> *some, *rest;
    public:

        append_node(list_node<Elem> *l1, list_node<Elem> *l2) {
            some = l1;
            rest = l2;
        }

        list_node<Elem> *copy_list() override;

        int len() override;

        Elem nth(int n);

        Elem nth_length(int n, int &len) override;

        void print() override {
            int i, size;
            size = len();
            std::cout <<  "append list node: " << std::endl;
            for (i = 0; i < size; i++)
                nth(i)->print();
        }

        Symbol semant(ExpressionTableP exprs) override;

        Value *codegen(ExpressionCodeTableP) override;

    };

    template <class Elem> Symbol nil_node<Elem>::semant(ExpressionTableP exprs) {
        std::cout << "semant() called on nil node" << std::endl;
        return nullptr;
    }

    template <class Elem> Symbol single_list_node<Elem>::semant(ExpressionTableP exprs) {
        std::cout << "semant() called on single list node" << std::endl;
        return elem->semant(exprs);
    }

    template <class Elem> Symbol append_node<Elem>::semant(ExpressionTableP exprs) {
        std::cout << "semant() called on append node" << std::endl;
        Symbol retVal = some->semant(exprs);
        Symbol restSym = rest->semant(exprs);
        std::cout << "append_node::semant() retVal = " << retVal << std::endl;
        std::cout << "append_node::semant() restSym = " << restSym << std::endl;
        return restSym == nullptr ? retVal : restSym;

    }

    template <class Elem> Value *nil_node<Elem>::codegen(ExpressionCodeTableP expCodeTab) {
        std::cout << "codegen() called on nil node" << std::endl;
        return nullptr;
    }

    template <class Elem> Value *single_list_node<Elem>::codegen(ExpressionCodeTableP expCodeTab) {
        std::cout << "codegen() called on single list node" << std::endl;
        return elem->codegen(expCodeTab);
    }

    template <class Elem> Value *append_node<Elem>::codegen(ExpressionCodeTableP expCodeTab) {
        std::cout << "codegen() called on append node" << std::endl;
        Value *retVal = some->codegen(expCodeTab);
        Value *restSym = rest->codegen(expCodeTab);
        std::cout << "append_node::codegen() retVal = " << retVal << std::endl;
        std::cout << "append_node::codegen() restSym = " << restSym << std::endl;
        return restSym == nullptr ? retVal : restSym;

    }






    template <class Elem> single_list_node<Elem> *list(Elem x);
    template <class Elem> append_node<Elem> *cons(Elem x, list_node<Elem> *l);
    template <class Elem> append_node<Elem> *xcons(list_node<Elem> *l, Elem x);


    template <class Elem> list_node<Elem> *list_node<Elem>::nil() { return new nil_node<Elem>(); }
    template <class Elem> list_node<Elem> *list_node<Elem>::single(Elem e) { return new single_list_node<Elem>(e); }
    template <class Elem> list_node<Elem> *list_node<Elem>::append(list_node<Elem> *l1,list_node<Elem> *l2) {
        return new append_node<Elem>(l1,l2);
    }

        ///////////////////////////////////////////////////////////////////////////
    //
    // list_node::nth
    //
    // function to find the nth element of the list
    //
    ///////////////////////////////////////////////////////////////////////////

    template <class Elem> Elem list_node<Elem>::nth(int n)
    {
        int len;
        Elem tmp = nth_length(n ,len);

        if (tmp)
            return tmp;
        else {
            cerr << "error: outside the range of the list\n";
            exit(1);
        }
    }

    // added 10/30/06 cgs
    template <class Elem> Elem append_node<Elem>::nth(int n) {
        int len;
        Elem tmp = nth_length(n ,len);

        if (tmp)
            return tmp;
        else {
            cerr << "error: outside the range of the list\n";
            exit(1);
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    //
    // nil_node::copy_list
    //
    // return the deep copy of the nil_node
    //
    ///////////////////////////////////////////////////////////////////////////
    template <class Elem> list_node<Elem> *nil_node<Elem>::copy_list() {
        return new nil_node<Elem>();
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // nil_node::len
    //
    // return the length of the nil_node
    //
    ///////////////////////////////////////////////////////////////////////////
    template <class Elem> int nil_node<Elem>::len() {
        return 0;
    }



    ///////////////////////////////////////////////////////////////////////////
    //
    // nil_node::nth_length
    //
    // return the nth element on the list
    //
    ///////////////////////////////////////////////////////////////////////////
    template <class Elem> Elem nil_node<Elem>::nth_length(int, int &len) {
        len = 0;
        return NULL;
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // single_list_node::copy_list
    //
    // return the deep copy of the single_list_node
    //
    ///////////////////////////////////////////////////////////////////////////
    template <class Elem> list_node<Elem> *single_list_node<Elem>::copy_list() {
        return new single_list_node<Elem>((Elem) elem->copy());
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // single_list_node::len
    //
    // return the length of the single_list_node
    //
    ///////////////////////////////////////////////////////////////////////////
    template <class Elem> int single_list_node<Elem>::len() {
        return 1;
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // single_list_node::nth_length
    //
    // return the nth element on the list
    //
    ///////////////////////////////////////////////////////////////////////////
    template <class Elem> Elem single_list_node<Elem>::nth_length(int n, int &len) {
        len = 1;
        if (n)
            return NULL;
        else
            return elem;
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // append_node::copy_list
    //
    // return the deep copy of the append_node
    //
    ///////////////////////////////////////////////////////////////////////////
    template <class Elem> list_node<Elem> *append_node<Elem>::copy_list() {
        return new append_node<Elem>(some->copy_list(), rest->copy_list());
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // append_node::len
    //
    // return the length of the append_node
    //
    ///////////////////////////////////////////////////////////////////////////
    template <class Elem> int append_node<Elem>::len() {
        return some->len() + rest->len();
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // append_node::nth_length
    //
    // return the nth element on the list
    //
    ///////////////////////////////////////////////////////////////////////////
    template <class Elem> Elem append_node<Elem>::nth_length(int n, int &len) {
        int rlen;
        Elem tmp = some->nth_length(n, len);

        if (!tmp) {
            tmp = rest->nth_length(n-len, rlen);
            len += rlen;
        }
        return tmp;
    }

    ///////////////////////////////////////////////////////////////////////////
    //
    // list
    //
    ///////////////////////////////////////////////////////////////////////////
    template <class Elem> single_list_node<Elem> *list(Elem x) {
        return new single_list_node<Elem>(x);
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // cons
    //
    ///////////////////////////////////////////////////////////////////////////
    template <class Elem> append_node<Elem> *cons(Elem x, list_node<Elem> *l) {
        return new append_node<Elem>(list(x), l);
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // xcons
    //
    ///////////////////////////////////////////////////////////////////////////
    template <class Elem> append_node<Elem> *xcons(list_node<Elem> *l, Elem x) {
        return new append_node<Elem>(l, list(x));
    }



    class block_node : public exp_node {
    protected:
        Expressions body;
    public:
        block_node(Expressions a1) {
            body = a1;
        }

        tree_node *copy() override { return nullptr; }
        void print() override  { body->print(); }

        Value *codegen(ExpressionCodeTableP) override;
//        Value *codegen() override { return nullptr; }

        Symbol semant(ExpressionTableP exprs) override;

    };

    class declaration_node: public exp_node {
    public:
        tree_node *copy() override { return nullptr; }
        Symbol type;
        Symbol name;
        exp_node *initialisation;

        void print() override  { std::cout << "declaration node: type: " << type->get_string() << ", variable name: " << name->get_string() << ", intitialisation: "; initialisation->print(); std::cout << std::endl; }

        Value *codegen(ExpressionCodeTableP) override;

        const Symbol getType() { return type; }
        const Symbol getName() { return name; }
        Expression getInit() { return initialisation; }

        declaration_node(Symbol t, Symbol n, exp_node *exp) : type(t), name(n), initialisation(exp) {  }

        Symbol semant(ExpressionTableP exprs) override;

    };

    class loop_node: public exp_node {
    public:
        Expression predicate;
        Expression body;

        tree_node *copy() override { return nullptr; }

        void print() override  { std::cout << "loop node: predicate: "; predicate->print(); std::cout << ", body: "; body->print(); std::cout << std::endl; }

        Value *codegen(ExpressionCodeTableP) override;

        loop_node(Expression p, Expression b) : predicate(p), body(b) { }

        Symbol semant(ExpressionTableP exprs) override;
    };

    class cond_node: public exp_node {
    public:
        Expression predicate;
        Expression if_body;
        Expression then_body;

        tree_node *copy() override { return nullptr; }

        void print() override  { std::cout << "conditional node: predicate: "; predicate->print(); std::cout << ", if body: "; if_body->print(); std::cout << ", then body: "; then_body->print();  std::cout << std::endl; }


        cond_node(Expression p, Expression ib, Expression tb) : predicate(p), if_body(ib), then_body(tb) { }
        Symbol semant(ExpressionTableP exprs) override;
        Value *codegen(ExpressionCodeTableP) override;
    };

    class function_node: public exp_node {
    public:
        tree_node *copy() override { return nullptr; }
        Symbol returnType;
        Symbol name;
        Formals formals;
        exp_node *body;
        Symbol semant(ExpressionTableP exprs) override;
        void publish(ExpressionTableP expressions);

        void print() override  {
            std::cout << "function node: return type: " << returnType->get_string() << ", function name: " << name->get_string() << norm << ", function params... " << std::endl;
            formals->print();
            std::cout << ", function body: ";
            body->print(); std::cout << std::endl;
        }

        Value *codegen(ExpressionCodeTableP) override;

        Function *function_codegen(ExpressionCodeTableP expCodeTab);
        Function *function_prototype_codegen(ExpressionCodeTableP expCodeTab);

        function_node(Symbol t, Symbol n, Formals formal_list, exp_node *exp) : returnType(t), name(n) { formals = formal_list; body = exp; }

    };

    class dispatch_node: public exp_node {
    public:
        tree_node *copy() override { return nullptr; }
        Symbol name;
        Expressions actuals;

        void print() override  {
            std::cout << "dispatch node: function name: " << name->get_string() << norm << ", function params... " << std::endl;
            actuals->print();
            std::cout << std::endl;
        }

        Value *codegen(ExpressionCodeTableP) override;

        dispatch_node(Symbol n, Expressions actual_list) : name(n), actuals(actual_list) {  }
        Symbol semant(ExpressionTableP exprs) override;

    };

    class integer_node : public exp_node {

    public:
        tree_node *copy() override { return nullptr; }
        Symbol sym;

        integer_node(Symbol value) : sym(value) {};

        void print() override;

        Value *codegen(ExpressionCodeTableP expCodeTab) override;
        Symbol semant(ExpressionTableP exprs) override;
    };

    class formal_node : public exp_node {

    public:
        tree_node *copy() override { return nullptr; }
        Symbol type;
        Symbol name;

        formal_node(Symbol t, Symbol n) : type(t), name(n) {};

        void print() override;

        Value *codegen(ExpressionCodeTableP expCodeTab) override;
        Symbol semant(ExpressionTableP exprs) override;
        Symbol get_name() { return name; }
        void publish(ExpressionTableP);
    };


    class operator_node : public exp_node {
    public:
        exp_node *left;
        exp_node *right;

        // the constructor for node links the node to its children,
        // and stores the character representation of the operator.
        operator_node(exp_node *L, exp_node *R) : left(L), right(R) {};
        Value *codegen(ExpressionCodeTableP expCodeTab) override;
        Symbol semant(ExpressionTableP exprs) override;
    };

    class unary_minus_node : public exp_node {
    protected:
        exp_node *_exp;
    public:
        tree_node *copy() override { return nullptr; }
        unary_minus_node(exp_node *exp) : _exp(exp) {};

        void print() override;

        Value *codegen(ExpressionCodeTableP expCodeTab) override;
        Symbol semant(ExpressionTableP exprs) override;
    };

    class id_node : public exp_node {

    protected:

        Symbol sym;

    public:

        tree_node *copy() override { return nullptr; }

        Symbol get_symbol() { return sym; }

        std::string get_string() {
            return sym->get_string();
        }

        id_node(Symbol value) : sym(value) {}

        void print() override;

        Value *codegen(ExpressionCodeTableP expCodeTab) override;
        Symbol semant(ExpressionTableP exprs) override;

    };

    class string_node : public exp_node {

    protected:

        Symbol sym;

    public:
        tree_node *copy() override { return nullptr; }

        string_node(Symbol value): sym(value)  {};

        void print() override;

        Value *codegen(ExpressionCodeTableP expCodeTab) override;
        Symbol semant(ExpressionTableP exprs) override;
    };

    class plus_node : public operator_node {

    public:

        // plus_node's constructor just uses node's constructor
        plus_node(exp_node *L, exp_node *R) : operator_node(L, R) {};

        void print() override;

        tree_node *copy() override { return nullptr; }

        Value *codegen(ExpressionCodeTableP expCodeTab) override;
        Symbol semant(ExpressionTableP exprs) override;
    };


    class minus_node : public operator_node {
    public:

        minus_node(exp_node *L, exp_node *R) : operator_node(L, R) {};

        void print() override;

        Value *codegen(ExpressionCodeTableP expCodeTab) override;
        Symbol semant(ExpressionTableP exprs) override;
    };


    class times_node : public operator_node {
    public:

        times_node(exp_node *L, exp_node *R) : operator_node(L, R) {};

        void print() override;

        Value *codegen(ExpressionCodeTableP expCodeTab) override;
        Symbol semant(ExpressionTableP exprs) override;
    };


    class divide_node : public operator_node {
    public:

        divide_node(exp_node *L, exp_node *R) : operator_node(L, R) {};

        void print() override;

        Value *codegen(ExpressionCodeTableP expCodeTab) override;
        Symbol semant(ExpressionTableP exprs) override;
    };

    class assignment_stmt : public exp_node {
    protected:
        id_node *_id;
        exp_node *exp;
    public:
        tree_node *copy() override { return nullptr; }


        assignment_stmt(Symbol name, exp_node *expr) { _id = new id_node(name); exp = expr; };

        void print() override;

        virtual Value *codegen(ExpressionCodeTableP expCodeTab) override;
        Symbol semant(ExpressionTableP exprs) override;
    };

    class print_stmt : public exp_node {
    protected:
        Symbol sym;
        exp_node *exp;
    public:
        tree_node *copy() override { return nullptr; }
        print_stmt(Symbol _sym) : sym(_sym) {};
        print_stmt(exp_node *_exp) : exp(_exp) {};

        void print() override { std::cout << "Print node: " << std::endl << "\tsym:" << sym << std::endl << "\texp: " << std::endl; exp->print(); std::cout << "End print node" << std::endl;  };

        virtual Value *codegen(ExpressionCodeTableP expCodeTab) override;
        Symbol semant(ExpressionTableP exprs) override;
    };


    Formals nil_Formals();
    Formals single_Formals(Formal);
    Formals append_Formals(Formals, Formals);

    Expressions nil_Expressions();
    Expressions single_Expressions(Expression);
    Expressions append_Expressions(Expressions, Expressions);

    Expression block(Expressions a1);

    Expression cond(Expression, Expression, Expression);
    Expression loop(Expression, Expression);

}


#endif