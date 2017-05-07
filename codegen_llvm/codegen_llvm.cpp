//
// Created by Kieran Rimmer on 3/5/17.
//

#include "codegen_llvm.hpp"

namespace RWL {


    Value *LogErrorV(const char *Str) {
        std::cerr << Str << std::endl;
        return nullptr;
    }

    void pgm::codegen() {
        ExpressionCodeTableP expCodeTab = new ExpressionCodeTable(exps);
        expCodeTab->CalleeF = (Function *) expCodeTab->TheModule->getOrInsertFunction("printf",
                                 FunctionType::get(
                                     IntegerType::getInt32Ty(TheContext),
                                     PointerType::get(Type::getInt8Ty(TheContext), 0),
                                     true /* this is var arg func type*/)
        );
        FunctionType *printf_type =
                TypeBuilder<int(char *, ...), false>::get(TheContext);

        Function *func = cast<Function>(expCodeTab->TheModule->getOrInsertFunction(
                "printf", printf_type)
        );
        expCodeTab->CalleeF = func;
        expCodeTab->llvm_values_.enterscope();
        exps->codegen(expCodeTab);
        expCodeTab->llvm_values_.exitscope();
        // Print out all of the generated code.
        expCodeTab->TheModule->print(errs(), nullptr);
    }

    Value *integer_node::codegen(ExpressionCodeTableP expCodeTab) {
        return ConstantInt::get(TheContext, APSInt(sym->get_string()));
    }

    Value *minus_node::codegen(ExpressionCodeTableP expCodeTab) {
        Value *L = left->codegen(expCodeTab);
        Value *R = right->codegen(expCodeTab);
        if (!L || !R) return nullptr;
        return expCodeTab->builder.CreateFSub(L, R, "subtmp");
    }

    Value *unary_minus_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    Value *print_stmt::codegen(ExpressionCodeTableP expCodeTab) {
        return expCodeTab->builder.CreateCall(expCodeTab->CalleeF, exp->codegen(expCodeTab), "printfCall");
    }

    Value *divide_node::codegen(ExpressionCodeTableP expCodeTab) {
        Value *L = left->codegen(expCodeTab);
        Value *R = right->codegen(expCodeTab);
        if (!L || !R) return nullptr;
        return expCodeTab->builder.CreateFDiv(L, R, "divtmp");
    }

    Value *times_node::codegen(ExpressionCodeTableP expCodeTab) {
        Value *L = left->codegen(expCodeTab);
        Value *R = right->codegen(expCodeTab);
        if (!L || !R) return nullptr;
        return expCodeTab->builder.CreateFMul(L, R, "multmp");
    }

    Value *plus_node::codegen(ExpressionCodeTableP expCodeTab) {
        Value *L = left->codegen(expCodeTab);
        Value *R = right->codegen(expCodeTab);
        if (!L || !R) return nullptr;
        return expCodeTab->builder.CreateFAdd(L, R, "addtmp");
    }

    Value *id_node::codegen(ExpressionCodeTableP expCodeTab) {
        return expCodeTab->llvm_values_.lookup(sym);
    }

    Value *string_node::codegen(ExpressionCodeTableP expCodeTab) {
        return ConstantDataArray::getString(TheContext, sym->get_string(), true);
    }

    Value *dispatch_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    Value *loop_node::codegen(ExpressionCodeTableP expCodeTab) {
        return nullptr;
    }

    Value *cond_node::codegen(ExpressionCodeTableP expCodeTab) {
        return nullptr;
    }

    Value *block_node::codegen(ExpressionCodeTableP expCodeTab) {
        expCodeTab->llvm_values_.enterscope();
        // do code gen
        Value *V = body->codegen(expCodeTab);
        expCodeTab->llvm_values_.exitscope();
        return V;
    }

    Value *declaration_node::codegen(ExpressionCodeTableP expCodeTab) {
        Value *V = initialisation->codegen(expCodeTab);
        std::cout << "Adding decalaration for variable: " << name << std::endl;
        expCodeTab->llvm_values_.addid(name, V);
        return V;
    }

    /** **

    Value *invoke_function_codegen(ExpressionCodeTableP expCodeTab, std::string Callee) {
        // Look up the name in the global module table.
        Function *CalleeF = expCodeTab->TheModule->getFunction(Callee);
        if (!CalleeF)
            return LogErrorV("Unknown function referenced");

        // If argument mismatch error.
        if (CalleeF->arg_size() != Args.size())
            return LogErrorV("Incorrect # arguments passed");

        std::vector<Value *> ArgsV;
        for (unsigned i = 0, e = Args.size(); i != e; ++i) {
            ArgsV.push_back(Args[i]->codegen());
            if (!ArgsV.back())
                return nullptr;
        }

        return expCodeTab->builder.CreateCall(CalleeF, ArgsV, "calltmp");
    }

    Function *function_prototype_codegen(ExpressionCodeTableP expCodeTab) {
        // Make the function type:  double(double,double) etc.
        std::vector<Type *> Doubles(Args.size(), Type::getDoubleTy(TheContext));
        FunctionType *FT =
                FunctionType::get(Type::getDoubleTy(TheContext), Doubles, false);

        Function *F =
                Function::Create(FT, Function::ExternalLinkage, Name, TheModule.get());

        // Set names for all arguments.
        unsigned Idx = 0;
        for (auto &Arg : F->args())
            Arg.setName(Args[Idx++]);

        return F;
    }

    Function *function_codegen(ExpressionCodeTableP expCodeTab) {
        // First, check for an existing function from a previous 'extern' declaration.
        Function *TheFunction = expCodeTab->TheModule->getFunction(Proto->getName());

        if (!TheFunction)
            TheFunction = Proto->codegen();

        if (!TheFunction)
            return nullptr;

        // Create a new basic block to start insertion into.
        BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
        Builder.SetInsertPoint(BB);

        // Record the function arguments in the NamedValues map.
        NamedValues.clear();
        for (auto &Arg : TheFunction->args())
            NamedValues[Arg.getName()] = &Arg;

        if (Value *RetVal = Body->codegen()) {
            // Finish off the function.
            Builder.CreateRet(RetVal);

            // Validate the generated code, checking for consistency.
            verifyFunction(*TheFunction);

            return TheFunction;
        }

        // Error reading body, remove function.
        TheFunction->eraseFromParent();
        return nullptr;
    }

    /**   **/

    Value *function_node::codegen(ExpressionCodeTableP expCodeTab) {

    }



    Value *assignment_stmt::codegen(ExpressionCodeTableP expCodeTab) {
        Value *V = ( expCodeTab->llvm_values_.lookup(_id->get_symbol()) );
        if (!V) {
            std::cerr << red <<  "Unknown variable name: " << norm << _id->get_string() << std::endl;
            return nullptr;
        }
        return V;
    }

    Value *operator_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    Value *formal_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

}