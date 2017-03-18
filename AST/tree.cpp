#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "tree.hpp"


namespace RWL {

  // the constructor for node links the node to its children,
  // and stores the character representation of the operator.
  operator_node::operator_node(exp_node *L, exp_node *R) {
    left    = L;
    right   = R;
  }

  
  number_node::number_node(int value) {
    num = value;
   }
  
void number_node:: print() {
  std::cout << num;
}

  int number_node::evaluate() { 
    std::cout << "number_node: operand = " << num << std::endl;
    return num; }


    integer_node::integer_node(char *value) {
    std::cout << "integer_node: value = " << value << std::endl;
    num = std::stoi(value);
   }

   integer_node::integer_node(Symbol value) {
    std::cout << "integer_node: value = " << value << std::endl;
    num = std::stoi(value->get_string());
   }
  
void integer_node:: print() {
  std::cout << num;
}

  int integer_node::evaluate() { 
    std::cout << "number_node: operand = " << num << std::endl;
    return num; }

  id_node::id_node(std::string value) : id(value) {}

  id_node::id_node(Symbol value) : id((std::string) value->get_string()) {}

void id_node:: print() {
  std::cout << id;
}

  int id_node::evaluate() { 
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

  int plus_node::evaluate() {
    int left_num, right_num;

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

  int minus_node::evaluate() {
    int left_num, right_num;

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

  int times_node::evaluate() {
    int left_num, right_num;

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

  int divide_node::evaluate() {
    int left_num, right_num;

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

int unary_minus_node::evaluate() {
  int expValue = exp->evaluate();
  num = -expValue;

  std::cout << "unary_minus_node: " << "-\t" << expValue << " = " << num << "\n";
    return num;
}

assignment_stmt::assignment_stmt(std::string name, exp_node *expression)
  : id(name), exp(expression) {}

assignment_stmt::assignment_stmt(Symbol symbol, exp_node *expression)
  : id( (std::string) symbol->get_string()), exp(expression) {}

void assignment_stmt::print() {
  std::cout << id << " = ";
  exp->print();
  std::cout << std::endl;
}

void assignment_stmt::evaluate() {
  int result = exp->evaluate();
  std::cout << "assignment_node: " << id << " = " << result << std::endl << std::endl;
  idTable[id] = result;
}

print_stmt::print_stmt (std::string name) : id(name) {}

print_stmt::print_stmt (int num) : id(std::to_string(num)) {}

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