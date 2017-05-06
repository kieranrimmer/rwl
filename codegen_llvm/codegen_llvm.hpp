//
// Created by Kieran Rimmer on 3/5/17.
//

#ifndef RWL_CODEGEN_LLVM_HPP
#define RWL_CODEGEN_LLVM_HPP

#include "../AST/tree.hpp"

namespace RWL {

    LLVMContext TheContext;

    class ExpressionCodeTable {
    private:
        int codegen_errors;
        ostream& error_stream;
        Expressions expressions_;
        Expression current_expression_;

    public:
        ExpressionCodeTable(Expressions exps) : codegen_errors(0), error_stream(std::cerr), builder(TheContext), expressions_(exps) {
            TheModule = llvm::make_unique<Module>("my cool jit", TheContext);
        };

        ostream& codegen_error();
        ostream& codegen_error(Expression exp);
        ostream& codegen_error(Symbol filename, tree_node *t);

        Expression get_expression(Symbol name);

        void set_current_expression(Expression exp) { current_expression_ = exp; }
        Expression get_current_expression() { return current_expression_; }




        IRBuilder<> builder;
        std::unique_ptr <Module> TheModule;
        SymbolTable<Symbol, Value> llvm_values_;

    };
}

#endif //RWL_CODEGEN_LLVM_HPP
