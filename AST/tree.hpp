
#ifndef _TREE_H_
#define _TREE_H_

#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "llvm/ADT/APFloat.h"
//#include "llvm/ADT/STLExtras.h"
//#include "llvm/IR/BasicBlock.h"
//#include "llvm/IR/Constants.h"
//#include "llvm/IR/DerivedTypes.h"
//#include "llvm/IR/Function.h"
//#include "llvm/IR/IRBuilder.h"
//#include "llvm/IR/LLVMContext.h"
//#include "llvm/IR/Module.h"
//#include "llvm/IR/Type.h"
//#include "llvm/IR/Verifier.h"

#include "../string_table/string_table.hpp"


namespace RWL{

template <class DataType> class exp_node {
  public:
    DataType num;

    // print function for pretty printing an expression
    virtual void print() = 0;

    // evaluation function for a leaf, replaced for interior nodes
    virtual DataType evaluate() = 0;
};

class operator_node : public exp_node<int> {
public:
    exp_node *left;
    exp_node *right;

  // the constructor for node links the node to its children,
  // and stores the character representation of the operator.
    operator_node(exp_node *L, exp_node *R);
};

class number_node : public exp_node<int> {
  
public:
  number_node(int value);
  void print();
  int evaluate();
};

class integer_node : public exp_node<int> {
  
public:
  integer_node(char *value);
  integer_node(Symbol value);
  void print();
  int evaluate();
};

class unary_minus_node : public exp_node<int> {
 protected:
  exp_node *exp;
 public:
  unary_minus_node(exp_node *exp);
  void print();
  int evaluate();
};

class id_node : public exp_node<int> {
protected:
  std::string id;

public:
  id_node(std::string value);
  id_node(Symbol value);
  void print();
  int evaluate();
};

class string_node : public exp_node<int> {
protected:
  std::string string_const;

public:
  string_node(std::string value);
  string_node(Symbol value);
  void print();
  int evaluate();
};

// plus_node inherits the characteristics of node and adds its own evaluate function
class plus_node : public operator_node {
  public:

  // plus_node's constructor just uses node's constructor
  plus_node(exp_node *L, exp_node *R);
  void print();
  int evaluate();
};


// minus_node inherits the characteristics of node and adds its own evaluate function
class minus_node : public operator_node {
  public:

  minus_node(exp_node *L, exp_node *R);
  void print();
  int evaluate();
};


// times_node inherits the characteristics of node and adds its own evaluate function
class times_node : public operator_node {
  public:

  times_node(exp_node *L, exp_node *R);
  void print();
  int evaluate();
};


// divide_node inherits the characteristics of node and adds its own evaluate function
class divide_node : public operator_node {
  public:

  divide_node(exp_node *L, exp_node *R);
  void print();
  int evaluate();
};

class statement {
 public:
  virtual void print() {}
  virtual void evaluate() = 0;
};

class assignment_stmt : public statement {
 protected:
  std::string id;
  exp_node<int> *exp;
 public:
  assignment_stmt(std::string name, exp_node<int> *expression);
  assignment_stmt(Symbol symbol, exp_node<int> *expression);
  void print();
  void evaluate();
};

class print_stmt: public statement {
 protected:
  std::string id;
 public:
  print_stmt(std::string id);
  print_stmt(int num); 
  void evaluate();
};

class pgm {
 protected:
  std::list<statement *> *stmts;
 public:
  pgm(std::list<statement *> *stmtlist);
  void evaluate();
};

// the object at the base of our tree
extern std::map<std::string, float> idTable;
extern RWL::pgm *root;

}


#endif