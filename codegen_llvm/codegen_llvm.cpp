//
// Created by Kieran Rimmer on 3/5/17.
//

#include "codegen_llvm.hpp"

namespace RWL {


    Value *LogErrorV(const char *Str) {
        std::cerr << Str << std::endl;
        return nullptr;
    }

    Function *LogErrorF(std::string Str) {
        std::cerr << Str << std::endl;
        return nullptr;
    }

    template <typename RetType> RetType *LogErrorAll(std::string Str) {
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
        return ConstantInt::get(Type::getInt32Ty(TheContext), sym->get_string(), 10);
//        return ConstantInt::get(TheContext, APSInt(sym->get_string()));
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

    Value *dispatch_node::codegen(ExpressionCodeTableP expCodeTab) {
        std::cout << "calling function `" << name->get_string() << "`" << std::endl;
        Function *CalleeF = expCodeTab->TheModule->getFunction(name->get_string());
        if (!CalleeF)
            return LogErrorAll<Value>("Unknown function referenced");

        // If argument mismatch error.
        if (CalleeF->arg_size() != actuals->len())
            return LogErrorV("Incorrect # arguments passed");

        std::vector<Value *> ArgsV;
        for (unsigned i = 0, e = actuals->len(); i != e; ++i) {
            Value *V = actuals->nth(i)->codegen(expCodeTab);
            std::cout << "Arg #" << i+1 << " type: ";
            V->getType()->dump();
            std::cout << std::endl;
            ArgsV.push_back(V);
            if (!ArgsV.back())
                return nullptr;
        }

        std::cout << "calling function with " << ArgsV.size() << " arguments" << std::endl;

        return expCodeTab->builder.CreateCall(CalleeF, ArgsV, "calltmp");
    }

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

    /** **/

    void generate_string_var(ExpressionCodeTableP expCodeTab) {
        ArrayType* ArrayTy_0 = ArrayType::get(IntegerType::get(expCodeTab->TheModule->getContext(), 8), 14);
        GlobalVariable *gvar_array__str = new GlobalVariable(
                *(expCodeTab->TheModule),
                ArrayTy_0,
                true,
                GlobalValue::PrivateLinkage,
                0, // has initializer, specified below
                ".str");
        gvar_array__str->setAlignment(1);

        // Constant Definitions
        Constant *const_array_4 = ConstantDataArray::getString(expCodeTab->TheModule->getContext(), "hello world", true);
        std::vector<Constant *> const_ptr_5_indices;
        ConstantInt* const_int64_6 = ConstantInt::get(TheContext, APInt(64, StringRef("0"), 10));
        const_ptr_5_indices.push_back((Constant *&&) const_int64_6);
        const_ptr_5_indices.push_back((Constant *&&) const_int64_6);
        Constant *const_ptr_5 = ConstantExpr::getGetElementPtr(ArrayTy_0, gvar_array__str, const_ptr_5_indices);

        // Global Variable Definitions
        gvar_array__str->setInitializer(const_array_4);
    }

    // Function Definitions

//    // Function: foo (func_foo)
//{
//
//    BasicBlock* label_entry = BasicBlock::Create(mod->getContext(), "entry",func_foo,0);
//
//    // Block entry (label_entry)
//    ReturnInst::Create(mod->getContext(), const_ptr_5, label_entry);
//
//}
//
//    /**
//     *
//     */
//    GlobalVariable* gvar_array__str = new GlobalVariable(
//            //Module=
//            *mod,
//    //Type=
//    ArrayTy_0,
//    //isConstant=
//    true,
//    //Linkage=
//    GlobalValue::PrivateLinkage,
//    //Initializer=
//    0, // has initializer, specified below
//    //Name=
//    ".str");
//    gvar_array__str->setAlignment(1);
//
//    // Constant Definitions
//    Constant *const_array_4 = ConstantDataArray::getString(mod->getContext(), "hello world", true);
//    std::vector<Constant*> const_ptr_5_indices;
//    ConstantInt* const_int64_6 = ConstantInt::get(mod->getContext(), APInt(64, StringRef("0"), 10));
//    const_ptr_5_indices.push_back(const_int64_6);
//    const_ptr_5_indices.push_back(const_int64_6);
//    Constant* const_ptr_5 = ConstantExpr::getGetElementPtr(gvar_array__str, const_ptr_5_indices);
//
//    // Global Variable Definitions
//    gvar_array__str->setInitializer(const_array_4);
//
//    // Function Definitions
//
//    // Function: foo (func_foo)
//{
//
//    BasicBlock* label_entry = BasicBlock::Create(mod->getContext(), "entry",func_foo,0);
//
//    // Block entry (label_entry)
//    ReturnInst::Create(mod->getContext(), const_ptr_5, label_entry);
//
//}

//     * **/

    Function *function_node::function_prototype_codegen(ExpressionCodeTableP expCodeTab) {
        // Make the function type:  double(double,double) etc.
        std::vector<Type *> our_formals;
        for(int i=0; i < formals->len(); i++) {
            if ( formals->nth(i)->type->get_string().compare("string") == 0 ) {
                our_formals.push_back(Type::getInt8PtrTy(TheContext));
                std::cout << "added string formal" << std::endl;
            }
            else if ( formals->nth(i)->type->get_string().compare("int") == 0 ) {
                our_formals.push_back(Type::getInt32Ty(TheContext));
                std::cout << "added int formal" << std::endl;
            }
            else {
                std::string f_type = formals->nth(i)->type->get_string();
                std::ostringstream stringStream;
                stringStream << "Cannot parse formal: " << formals->nth(i)->name->get_string() << " of type: " << f_type << std::endl;
                std::string copyOfStr = stringStream.str();
                return LogErrorF(copyOfStr);
            }
        }
        FunctionType *FT = nullptr;
        std::cout << "created FunctionType pointer as nullptr" << std::endl;
        std::cout << "return type for function is: `" << returnType->get_string() << "`" << std::endl;
        if (returnType->get_string().compare("int") == 0) {

            std::cout << "return type is int" << std::endl;
            FT =
            FunctionType::get(Type::getInt32Ty(TheContext), our_formals, false);
        }
        else if (returnType->get_string().compare("string") == 0) {
            std::cout << "return type is string" << std::endl;
            FT = FunctionType::get(Type::getInt8PtrTy(TheContext), our_formals, false);
        }
        std::cout << "created FuntionType pointer, FT = ";
//        FT->dump();
        std::cout << std::endl;

        Function *F =
                Function::Create(FT, Function::ExternalLinkage, name->get_string(), expCodeTab->TheModule.get());

        // Set names for all arguments.
        unsigned Idx = 0;
        for (auto &Arg : F->args())
            Arg.setName(formals->nth(Idx++)->get_name()->get_string());

        return F;
    }

    /** **/
    Function *function_node::function_codegen(ExpressionCodeTableP expCodeTab) {
        // First, check for an existing function from a previous 'extern' declaration.
        Function *TheFunction = expCodeTab->TheModule->getFunction(name->get_string());

        if (!TheFunction)
            TheFunction = function_prototype_codegen(expCodeTab);

        if (!TheFunction)
            return nullptr;

        // Create a new basic block to start insertion into.
        BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
        expCodeTab->builder.SetInsertPoint(BB);

        // Record the function arguments in the NamedValues map.
        expCodeTab->NamedValues.clear();
        for (auto &Arg : TheFunction->args())
            expCodeTab->NamedValues[Arg.getName()] = &Arg;

        if (Value *RetVal = body->codegen(expCodeTab)) {
            // Finish off the function.
            expCodeTab->builder.CreateRet(RetVal);

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
        function_codegen(expCodeTab);
        return nullptr;
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