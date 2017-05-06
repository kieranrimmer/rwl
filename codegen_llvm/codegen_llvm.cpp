//
// Created by Kieran Rimmer on 3/5/17.
//

#include "codegen_llvm.hpp"

namespace RWL {

    // IR code generation
//    static LLVMContext TheContext;
//    static IRBuilder<> Builder(TheContext);
//    static std::unique_ptr <Module> TheModule;
//    static std::map<std::string, Value *> NamedValues;
    static SymbolTable<Symbol, Value *> llvm_values_;

    void pgm::codegen() {
        llvm_values_.enterscope();
        LLVMContext TheContext;
        ExpressionCodeTableP expCodeTab = new ExpressionCodeTable(exps);
        exps->codegen(expCodeTab);
        llvm_values_.exitscope();
    }

    Value *integer_node::codegen(ExpressionCodeTableP expCodeTab) {
        return ConstantInt::get(TheContext, APSInt(sym->get_string()));
    }

    Value *minus_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    Value *unary_minus_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    Value *print_stmt::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    Value *divide_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    Value *times_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    Value *plus_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    Value *id_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    Value *string_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    Value *assignment_stmt::codegen(ExpressionCodeTableP expCodeTab) {
        return exp->codegen(expCodeTab);
    }

    Value *operator_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    Value *formal_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

}