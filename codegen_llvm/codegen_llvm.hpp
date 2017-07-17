//
// Created by Kieran Rimmer on 3/5/17.
//

#ifndef RWL_CODEGEN_LLVM_HPP
#define RWL_CODEGEN_LLVM_HPP

#include <sstream>



#include "../AST/tree.hpp"
#include "../type_check.hpp"

// #include "llvm/Analysis/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/TypeBuilder.h"
#include "llvm/IR/IRBuilder.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "include/KaleidoscopeJIT.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace llvm;
using namespace llvm::sys;
using namespace llvm::orc;

namespace RWL {

    LLVMContext TheContext;

    Module *Mod;

    Module *makeLLVMModule(ExpressionCodeTableP expCodeTab);


    // Initialize the target registry etc.


    class ExpressionCodeTable {
    private:
        int codegen_errors;
        ostream& error_stream;
        Expressions expressions_;
        Expression current_expression_;

        Function *currentFunction;

    public:
        ExpressionCodeTable(Expressions exps) : codegen_errors(0), error_stream(std::cerr), builder(TheContext), expressions_(exps) {
            TheModule = llvm::make_unique<Module>("test.ll", TheContext);

        };

        int generateObjectCode();

        ostream& codegen_error();
        ostream& codegen_error(Expression exp);
        ostream& codegen_error(Symbol filename, tree_node *t);

        Expression get_expression(Symbol name);

        void set_current_expression(Expression exp) { current_expression_ = exp; }
        Expression get_current_expression() { return current_expression_; }

        Function *getCurrentFunction() {
            return currentFunction;
        }

        void setCurrentFunction(Function *function) {
            currentFunction = function;
        }








        IRBuilder<> builder;
        std::unique_ptr <Module> TheModule;
        SymbolTable<Symbol, Value> llvm_values_;
        std::map<std::string, Value *> NamedValues; // for function formals
        Constant *CalleeF;

    };


    void generate_string_var(ExpressionCodeTableP expCodeTab);

}

#endif //RWL_CODEGEN_LLVM_HPP
