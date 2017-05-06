//
// Created by Kieran Rimmer on 3/5/17.
//

#include "codegen_llvm.hpp"

namespace RWL {

    void pgm::codegen() {
        ExpressionCodeTableP expCodeTab = new ExpressionCodeTable(exps);
        expCodeTab->llvm_values_.enterscope();
        exps->codegen(expCodeTab);
        expCodeTab->llvm_values_.exitscope();
        // Print out all of the generated code.
        expCodeTab->TheModule->print(errs(), nullptr);
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

    Value *dispatch_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }
    Value *loop_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }
    Value *cond_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }
    Value *block_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }
    Value *declaration_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }
    Value *function_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }



    Value *assignment_stmt::codegen(ExpressionCodeTableP expCodeTab) {
        Value *V = ( expCodeTab->llvm_values_.lookup(_id->get_symbol()) );
        if (!V) {
            std::cerr << "Unknown variable name";
            return nullptr;
        }
        return V;
    }

    Value *operator_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    Value *formal_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

}