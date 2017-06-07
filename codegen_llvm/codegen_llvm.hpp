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

using namespace llvm;
using namespace llvm::sys;

namespace RWL {

    LLVMContext TheContext;


    // Initialize the target registry etc.


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

        int generateObjectCode() {
            std::cout << "generateObjectCode() called" << std::endl;
            InitializeAllTargetInfos();
            InitializeAllTargets();
            InitializeAllTargetMCs();
            InitializeAllAsmParsers();
            InitializeAllAsmPrinters();

            auto TargetTriple = sys::getDefaultTargetTriple();
            TheModule->setTargetTriple(TargetTriple);

            std::string Error;
            auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

            // Print an error and exit if we couldn't find the requested target.
            // This generally occurs if we've forgotten to initialise the
            // TargetRegistry or we have a bogus target triple.
            if (!Target) {
                errs() << Error;
                return 1;
            }

            auto CPU = "generic";
            auto Features = "";

            TargetOptions opt;
            auto RM = Optional<Reloc::Model>();
            auto TheTargetMachine =
                    Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

            TheModule->setDataLayout(TheTargetMachine->createDataLayout());

            auto Filename = "output.o";
            std::error_code EC;
            raw_fd_ostream dest(Filename, EC, sys::fs::F_None);

            if (EC) {
                errs() << "Could not open file: " << EC.message();
                return 1;
            }

            legacy::PassManager pass;
            auto FileType = TargetMachine::CGFT_ObjectFile;

            if (TheTargetMachine->addPassesToEmitFile(pass, dest, FileType)) {
                errs() << "TheTargetMachine can't emit a file of this type";
                return 1;
            }

            std::cout << "preparing to run object code generation..." << std::endl;

            pass.run(*TheModule);
            dest.flush();

            outs() << "Wrote " << Filename << "\n";
        }

        ostream& codegen_error();
        ostream& codegen_error(Expression exp);
        ostream& codegen_error(Symbol filename, tree_node *t);

        Expression get_expression(Symbol name);

        void set_current_expression(Expression exp) { current_expression_ = exp; }
        Expression get_current_expression() { return current_expression_; }








        IRBuilder<> builder;
        std::unique_ptr <Module> TheModule;
        SymbolTable<Symbol, Value> llvm_values_;
        std::map<std::string, Value *> NamedValues; // for function formals
        Function *CalleeF;

    };


    void generate_string_var(ExpressionCodeTableP expCodeTab);

}

#endif //RWL_CODEGEN_LLVM_HPP
