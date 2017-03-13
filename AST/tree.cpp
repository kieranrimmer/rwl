#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "exp.hpp"


namespace RWL {

  // the constructor for node links the node to its children,
  // and stores the character representation of the operator.
  operator_node::operator_node(exp_node *L, exp_node *R) {
    left    = L;
    right   = R;
  }

  
  number_node::number_node(float value) {
    num = value;
   }
  
void number_node:: print() {
  std::cout << num;
}

  float number_node::evaluate() { 
    std::cout << "number_node: operand = " << num << std::endl;
    return num; }

  id_node::id_node(std::string value) : id(value) {}

void id_node:: print() {
  std::cout << id;
}

  float id_node::evaluate() { 
    std::cout << "id_node: " << id << " = " << idTable[id] << std::endl;
    return idTable[id]; 
  }

// plus_node inherits the characteristics of node and adds its own evaluate function
  // plus_node's constructor just uses node's constructor
  plus_node::plus_node(exp_node *L, exp_node *R) : operator_node(L,R) {
  }

void plus_node:: print() {
  std::cout << "(";
  left->print();
  std::cout << " + ";
  right->print();
  std::cout << ")";
}

  float plus_node::evaluate() {
    float left_num, right_num;

    left_num  = left->evaluate();
    right_num = right->evaluate();

    num = left_num + right_num;
    std::cout << "plus_node: " << left_num << " + " << right_num << " = " << num << "\n";
    return (num);
  }


// minus_node inherits the characteristics of node and adds its own evaluate function
  minus_node::minus_node(exp_node *L, exp_node *R) : operator_node(L,R) {
  }

void minus_node:: print() {
  std::cout << "(";
  left->print();
  std::cout << " - ";
  right->print();
  std::cout << ")";
}

  float minus_node::evaluate() {
    float left_num, right_num;

    left_num  = left->evaluate();
    right_num = right->evaluate();

    num = left_num - right_num;
    std::cout << "minus_node: " << left_num << " - " << right_num << " = " << num << "\n";
    return (num);
  }


// times_node inherits the characteristics of node and adds its own evaluate function
  times_node::times_node(exp_node *L, exp_node *R) : operator_node(L,R) {
  }

void times_node:: print() {
  std::cout << "(";
  left->print();
  std::cout << " * ";
  right->print();
  std::cout << ")";
}

  float times_node::evaluate() {
    float left_num, right_num;

    left_num = left->evaluate();
    right_num = right->evaluate();

    num = left_num * right_num;
    std::cout << "times_node: " << left_num << " * " << right_num << " = " << num << "\n";
    return (num);
  }


// divide_node inherits the characteristics of node and adds its own evaluate function

  divide_node::divide_node(exp_node *L, exp_node *R) : operator_node(L,R) {
  }

void divide_node:: print() {
  std::cout << "(";
  left->print();
  std::cout << " / ";
  right->print();
  std::cout << ")";
}

  float divide_node::evaluate() {
    float left_num, right_num;

    left_num = left->evaluate();
    right_num = right->evaluate();

    if(right_num)
      {
        num = (float)left_num / (float)right_num;
        std::cout << "divide_node: " << left_num << " / " << right_num << " = " << num << "\n";
        return (num);
      }
    else
      {
  std::cout << "divide_node: division by zero -> " << left_num << " / " << 0 << std::endl;
  // you have to include stdlib.h for exit
  exit(1);
      }
  }

// unary_minus_node inherits the characteristics of node and adds its own evaluate function
unary_minus_node::unary_minus_node(exp_node *L) : exp(L) {}

void unary_minus_node:: print() {
  std::cout << "-";
  exp->print();
}

float unary_minus_node::evaluate() {
  int expValue = exp->evaluate();
  num = -expValue;

  std::cout << "unary_minus_node: " << "-\t" << expValue << " = " << num << "\n";
    return num;
}

assignment_stmt::assignment_stmt(std::string name, exp_node *expression)
  : id(name), exp(expression) {}

void assignment_stmt::print() {
  std::cout << id << " = ";
  exp->print();
  std::cout << std::endl;
}

void assignment_stmt::evaluate() {
  float result = exp->evaluate();
  std::cout << "assignment_node: " << id << " = " << result << std::endl << std::endl;
  idTable[id] = result;
}

print_stmt::print_stmt (std::string name) : id(name) {}

void print_stmt::evaluate() {
  std::cout << "print_node: " << id << " = " << idTable[id] << std::endl << std::endl;
}

pgm::pgm(std::list<statement *> *stmtList) : stmts(stmtList) {}

void pgm::evaluate() {
  std::list<statement *>::iterator stmtIter;
  for (stmtIter = stmts->begin(); stmtIter != stmts->end();
       stmtIter++) {
    (*stmtIter)->print();
    (*stmtIter)->evaluate();
  }
}

std::map<std::string, float> idTable;

}