//
// Created by Kieran Rimmer on 3/5/17.
//

#include "codegen_llvm.hpp"

namespace RWL {

    // IR code generation
    static LLVMContext TheContext;
    static IRBuilder<> Builder(TheContext);
    static std::unique_ptr <Module> TheModule;
    static std::map<std::string, Value *> NamedValues;

    void pgm::codegen() {
        exps->codegen();
    }

    Value *integer_node::codegen() {
        return ConstantInt::get(TheContext, APSInt(sym->get_string()));
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

    Value *formal_node::codegen() { return nullptr; }

}