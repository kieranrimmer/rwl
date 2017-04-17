
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
#include "../util/util.hpp"

using Value=llvm::Value;

using namespace llvm;

namespace RWL {

    extern std::string pad(int n);

    class tree_node {
    protected:
        int line_no;
    public:
    };

    class exp_node: public tree_node {
    public:

        virtual tree_node *copy() = 0;

        // print function for pretty printing an expression
        virtual void print() = 0;

        // evaluation function for a leaf, replaced for interior nodes
        virtual void evaluate() = 0;

        virtual Value *codegen() = 0;

        virtual void dump(ostream& stream, int n) { stream << "dumping from exp_node" << std::endl; print(); };

        virtual int len() { return 0; }
    };

    typedef exp_node *Expression;



    template <class Elem> class list_node: public exp_node {
    public:
//        std::list<Elem> *elements;
//
//        list_node(std::list<Elem> *explist) : elements(explist) {}

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
            evaluate();
        }

        void evaluate() override {
//            typename std::list<Elem>::iterator expIter;
            std::cout << red << "templated list node (block), with  formal parameters" << norm << std::endl;
//            for (expIter = elements->begin(); expIter != elements->end();
//                 expIter++) {
//                std::cout << blue << "@ element #" << (*expIter) << norm << std::endl;
//                (*expIter)->print();
//                (*expIter)->evaluate();
//            }
        }

        Value *codegen() override { return nullptr; }

    };





    template <class Elem> class nil_node : public list_node<Elem> {
    public:
        list_node<Elem> *copy_list() override;
        int len() override;
        Elem nth_length(int n, int &len) override;
        void print() override { std::cout <<  "nil node" << std::endl; }
        void dump(ostream& stream, int n) override;
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
        void dump(ostream& stream, int n) override;
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
        void dump(ostream& stream, int n) override;
    };


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
    template <class Elem> Elem append_node<Elem>::nth(int n)
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

///////////////////////////////////////////////////////////////////////////
//
// nil_node::copy_list
//
// return the deep copy of the nil_node
//
///////////////////////////////////////////////////////////////////////////
    template <class Elem> list_node<Elem> *nil_node<Elem>::copy_list()
    {
        return new nil_node<Elem>();
    }


///////////////////////////////////////////////////////////////////////////
//
// nil_node::len
//
// return the length of the nil_node
//
///////////////////////////////////////////////////////////////////////////
    template <class Elem> int nil_node<Elem>::len()
    {
        return 0;
    }



///////////////////////////////////////////////////////////////////////////
//
// nil_node::nth_length
//
// return the nth element on the list
//
///////////////////////////////////////////////////////////////////////////
    template <class Elem> Elem nil_node<Elem>::nth_length(int, int &len)
    {
        len = 0;
        return NULL;
    }


///////////////////////////////////////////////////////////////////////////
//
// nil_node::dump
//
// dump for list node
//
///////////////////////////////////////////////////////////////////////////
    template <class Elem> void nil_node<Elem>::dump(ostream& stream, int n)
    {
        stream << pad(n) << "(nil)\n";
    }


///////////////////////////////////////////////////////////////////////////
//
// single_list_node::copy_list
//
// return the deep copy of the single_list_node
//
///////////////////////////////////////////////////////////////////////////
    template <class Elem> list_node<Elem> *single_list_node<Elem>::copy_list()
    {
        return new single_list_node<Elem>((Elem) elem->copy());
    }


///////////////////////////////////////////////////////////////////////////
//
// single_list_node::len
//
// return the length of the single_list_node
//
///////////////////////////////////////////////////////////////////////////
    template <class Elem> int single_list_node<Elem>::len()
    {
        return 1;
    }


///////////////////////////////////////////////////////////////////////////
//
// single_list_node::nth_length
//
// return the nth element on the list
//
///////////////////////////////////////////////////////////////////////////
    template <class Elem> Elem single_list_node<Elem>::nth_length(int n, int &len)
    {
        len = 1;
        if (n)
            return NULL;
        else
            return elem;
    }


///////////////////////////////////////////////////////////////////////////
//
// single_list_node::dump
//
// dump for list node
//
///////////////////////////////////////////////////////////////////////////
    template <class Elem> void single_list_node<Elem>::dump(ostream& stream, int n)
    {
        elem->print();
        stream << "indside dump() for single list node" << std::endl;
        elem->dump(stream, n);
    }


///////////////////////////////////////////////////////////////////////////
//
// append_node::copy_list
//
// return the deep copy of the append_node
//
///////////////////////////////////////////////////////////////////////////
    template <class Elem> list_node<Elem> *append_node<Elem>::copy_list()
    {
        return new append_node<Elem>(some->copy_list(), rest->copy_list());
    }


///////////////////////////////////////////////////////////////////////////
//
// append_node::len
//
// return the length of the append_node
//
///////////////////////////////////////////////////////////////////////////
    template <class Elem> int append_node<Elem>::len()
    {
        return some->len() + rest->len();
    }


///////////////////////////////////////////////////////////////////////////
//
// append_node::nth_length
//
// return the nth element on the list
//
///////////////////////////////////////////////////////////////////////////
    template <class Elem> Elem append_node<Elem>::nth_length(int n, int &len)
    {
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
// append_node::dump
//
// dump for list node
//
///////////////////////////////////////////////////////////////////////////
    template <class Elem> void append_node<Elem>::dump(ostream& stream, int n)
    {
        int i, size;

        size = len();
        stream << pad(n) << "list\n";
        for (i = 0; i < size; i++)
//            std::cout << "i = " << i << std::endl;
            nth(i)->dump(stream, n+2);
        stream << pad(n) << "(end_of_list)\n";
    }


///////////////////////////////////////////////////////////////////////////
//
// list
//
///////////////////////////////////////////////////////////////////////////
    template <class Elem> single_list_node<Elem> *list(Elem x)
    {
        return new single_list_node<Elem>(x);
    }


///////////////////////////////////////////////////////////////////////////
//
// cons
//
///////////////////////////////////////////////////////////////////////////
    template <class Elem> append_node<Elem> *cons(Elem x, list_node<Elem> *l)
    {
        return new append_node<Elem>(list(x), l);
    }


///////////////////////////////////////////////////////////////////////////
//
// xcons
//
///////////////////////////////////////////////////////////////////////////
    template <class Elem> append_node<Elem> *xcons(list_node<Elem> *l, Elem x)
    {
        return new append_node<Elem>(l, list(x));
    }

    typedef list_node<Expression> Expressions_class;
    typedef Expressions_class *Expressions;

    class block_node : public exp_node {
    protected:
        Expressions body;
    public:
        block_node(Expressions a1) {
            body = a1;
        }

        tree_node *copy() override { return nullptr; }
        void print() override  { body->print(); }

        void evaluate() override { print(); }
        Value *codegen() override { return nullptr; }
        void dump(ostream& stream, int n) override {}

    };

    class declaration_node: public exp_node {
    public:
        tree_node *copy() override { return nullptr; }
        Symbol type;
        Symbol name;
        exp_node *initialisation;

        void print() override  { std::cout << "declaration node: type: " << type->get_string() << ", variable name: " << name->get_string() << ", intitialisation: "; initialisation->print(); std::cout << std::endl; }

        void evaluate() override { print(); }

        Value *codegen() override { return nullptr; }
        void dump(ostream& stream, int n) override {}

        declaration_node(Symbol t, Symbol n, exp_node *exp) : type(t), name(n) { initialisation = exp; }
    };

    class function_node: public exp_node {
    public:
        tree_node *copy() override { return nullptr; }
        Symbol returnType;
        Symbol name;
        Expressions formals;
        exp_node *body;
        void dump(ostream& stream, int n) override {}

        void print() override  {
            std::cout << "function node: return type: " << returnType->get_string() << ", function name: " << name->get_string() << norm << ", function params... " << std::endl;
            formals->print();
            std::cout << ", function body: ";
            body->print(); std::cout << std::endl;
        }

        void evaluate() override { print(); }

        Value *codegen() override { return nullptr; }

        function_node(Symbol t, Symbol n, Expressions formal_list, exp_node *exp) : returnType(t), name(n) { formals = formal_list; body = exp; }

    };

    class dispatch_node: public exp_node {
    public:
        tree_node *copy() override { return nullptr; }
        Symbol name;
        Expressions actuals;
        void dump(ostream& stream, int n) override {}

        void print() override  {
            std::cout << "dispatch node: function name: " << name->get_string() << norm << ", function params... " << std::endl;
            actuals->print();
            std::cout << std::endl;
        }

        void evaluate() override { print(); }

        Value *codegen() override { return nullptr; }

        dispatch_node(Symbol n, Expressions actual_list) : name(n) { actuals = actual_list; }

    };

    class integer_node : public exp_node {

    public:
        tree_node *copy() override { return nullptr; }
        Symbol sym;

        integer_node(Symbol value) : sym(value) {};

        void print() override;

        void evaluate() override;
        void dump(ostream& stream, int n) override {}

        Value *codegen() override;
    };

    class formal_node : public exp_node {

    public:
        tree_node *copy() override { return nullptr; }
        Symbol type;
        Symbol name;

        formal_node(Symbol t, Symbol n) : type(t), name(n) {};

        void print() override;

        void evaluate() override;
        void dump(ostream& stream, int n) override {}

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
        void dump(ostream& stream, int n) override {}
    };

    class unary_minus_node : public exp_node {
    protected:
        exp_node *_exp;
    public:
        tree_node *copy() override { return nullptr; }
        unary_minus_node(exp_node *exp) : _exp(exp) {};

        void print() override;

        void evaluate() override;

        Value *codegen() override;
        void dump(ostream& stream, int n) override {}
    };

    class id_node : public exp_node {

    protected:

        Symbol sym;

    public:

        tree_node *copy() override { return nullptr; }

        Symbol get_symbol() { return sym; }

        id_node(Symbol value) : sym(value) {}

        void print() override;

        void evaluate() override;

        Value *codegen() override;

        void dump(ostream& stream, int n) override {}
    };

    class string_node : public exp_node {

    protected:

        Symbol sym;

    public:
        tree_node *copy() override { return nullptr; }

        string_node(Symbol value): sym(value)  {};

        void print() override;

        void evaluate() override;

        Value *codegen() override;
        void dump(ostream& stream, int n) override {}
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
        tree_node *copy() override { return nullptr; }


        assignment_stmt(Symbol name, exp_node *expr) { _id = new id_node(name); exp = expr; };
        void dump(ostream& stream, int n) override {}

        void print() override;

        void evaluate() override;
        virtual Value *codegen() override;
    };

    class print_stmt : public exp_node {
    protected:
        Symbol sym;
        exp_node *exp;
    public:
        tree_node *copy() override { return nullptr; }
        print_stmt(Symbol _sym) : sym(_sym) {};
        print_stmt(exp_node *_exp) : exp(_exp) {};
        void dump(ostream& stream, int n) override {}

        void print() override { std::cout << "Print node: " << std::endl << "\tsym:" << sym << std::endl << "\texp: " << std::endl; exp->print(); std::cout << "End print node" << std::endl;  };

        virtual void evaluate() override;
        virtual Value *codegen() override;
    };

    class pgm {
    protected:
        Expressions exps;
    public:
        pgm(Expressions explist) { exps = explist; }

        void evaluate();
        Value *codegen();
    };

// the object at the base of our tree
    extern RWL::pgm *root;


    Expressions nil_Expressions();
    Expressions single_Expressions(Expression);
    Expressions append_Expressions(Expressions, Expressions);

    Expression block(Expressions a1);

}


#endif