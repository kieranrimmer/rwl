//
// Created by Kieran Rimmer on 3/5/17.
//

#include "codegen_llvm.hpp"

namespace RWL {

    Constant* geti8StrVal(Module& M, char const* str, Twine const& name, ExpressionCodeTableP expCodeTab) {
        LLVMContext& ctx = TheContext;
        Constant* strConstant = ConstantDataArray::getString(ctx, str);
        GlobalVariable* GVStr =
                new GlobalVariable(M, strConstant->getType(), true,
                                   GlobalValue::InternalLinkage, strConstant, name);
        Constant* zero = Constant::getNullValue(IntegerType::getInt32Ty(ctx));
        Constant* indices[] = {zero, zero};
        ArrayType* ArrayTy_0 = ArrayType::get(IntegerType::get(expCodeTab->TheModule->getContext(), 8), 14);
        Constant* strVal = ConstantExpr::getGetElementPtr(ArrayTy_0, GVStr, indices);

        return strVal;
    }


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

//        const Type *SBP = PointerType::get(Type::SByteTy);
//        const FunctionType *MTy =
//                FunctionType::get(Type::IntTy, std::vector<const Type*>(1, SBP), true);
//        PrintfFunc = M.getOrInsertFunction("printf", MTy);

//
        ExpressionCodeTableP expCodeTab = new ExpressionCodeTable(exps);
        expCodeTab->CalleeF = expCodeTab->TheModule->getOrInsertFunction("printf",
                                 FunctionType::get(
                                     IntegerType::getInt32Ty(TheContext),
                                     PointerType::get(Type::getInt8Ty(TheContext), 0),
                                     true /* this is var arg func type*/)
        );
//        FunctionType *printf_type =
//                TypeBuilder<int(char *, ...), false>::get(TheContext);
//
//        Function *func = cast<Function>(expCodeTab->TheModule->getOrInsertFunction(
//                "printf", printf_type)
//        );
//        expCodeTab->CalleeF = func;
        expCodeTab->llvm_values_.enterscope();
        exps->codegen(expCodeTab);
        expCodeTab->llvm_values_.exitscope();
        // Print out all of the generated code.
        expCodeTab->TheModule->print(errs(), nullptr);
        expCodeTab->TheModule->dump();
        generate_string_var(expCodeTab);

        expCodeTab->generateObjectCode();
        std::cout << "Object code generation complete" << std::endl;
    }




    int ExpressionCodeTable::generateObjectCode() {
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

        pass.add(createVerifierPass());

        if (TheTargetMachine->addPassesToEmitFile(pass, dest, FileType)) {
            errs() << "TheTargetMachine can't emit a file of this type";
            return 1;
        }

        std::cout << "preparing to run object code generation..." << std::endl;


        pass.run(*TheModule);
        dest.flush();

        outs() << "Wrote " << Filename << "\n";
        return 0;
    }





    Value *integer_node::codegen(ExpressionCodeTableP expCodeTab) {
        std::cout << "integer node codegen called" << std::endl;
        return ConstantInt::get(Type::getInt32Ty(TheContext), StringRef(sym->get_string()), (u_int8_t) 10);
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

        std::cout <<  "print_stmt::codegen(), type = " << std::endl;



        RwlTypes rwlType = TypeCheck::checkType(exp->get_type());

        switch (TypeCheck::checkType(exp->get_type())) {
            case RwlTypes::STRING: {

                std::string strValue = exp->get_value()->get_string();
                ArrayType *ArrayTy_0 = ArrayType::get(IntegerType::get(expCodeTab->TheModule->getContext(), 8), strValue.length() + 1);
                Constant *const_array_4 = ConstantDataArray::getString(expCodeTab->TheModule->getContext(),
                                                                       strValue.c_str());
                const_array_4->getType()->dump();
                ArrayTy_0->dump();
                GlobalVariable *gvar_array__str = new GlobalVariable(
                        *(expCodeTab->TheModule),
                        ArrayTy_0,
                        true,
                        GlobalValue::PrivateLinkage,
                        const_array_4, // has initializer, specified below
                        ".str");
                gvar_array__str->setAlignment(1);

                std::vector<Constant *> const_ptr_5_indices;
                ConstantInt *const_int64_6 = ConstantInt::get(TheContext, APInt(32, 0, false));
                const_ptr_5_indices.push_back((Constant *&&) const_int64_6);
                const_ptr_5_indices.push_back((Constant *&&) const_int64_6);
                Constant *const_ptr_5 = ConstantExpr::getGetElementPtr(ArrayTy_0, gvar_array__str, const_ptr_5_indices);
                return expCodeTab->builder.CreateCall(expCodeTab->CalleeF, const_ptr_5);

            }

            case RwlTypes::INT: {
                ArrayType *ArrayTy_0 = ArrayType::get(IntegerType::get(expCodeTab->TheModule->getContext(), 8), 5);
                Constant *const_array_4 = ConstantDataArray::getString(expCodeTab->TheModule->getContext(),
                                                                       "%d/n");
                const_array_4->getType()->dump();
                ArrayTy_0->dump();
                GlobalVariable *gvar_array__str = new GlobalVariable(
                        *(expCodeTab->TheModule),
                        ArrayTy_0,
                        true,
                        GlobalValue::PrivateLinkage,
                        const_array_4, // has initializer, specified below
                        ".int");
                gvar_array__str->setAlignment(1);

                std::vector<Constant *> const_ptr_5_indices;
                ConstantInt *const_int64_6 = ConstantInt::get(TheContext, APInt(32, 0, false));
                const_ptr_5_indices.push_back((Constant *&&) const_int64_6);
                const_ptr_5_indices.push_back((Constant *&&) const_int64_6);
                Constant *const_ptr_5 = ConstantExpr::getGetElementPtr(ArrayTy_0, gvar_array__str, const_ptr_5_indices);
                return expCodeTab->builder.CreateCall(expCodeTab->CalleeF, {const_ptr_5, ConstantInt::get(TheContext, APInt(32, 22)) } );

            }

        }
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
        std::cout << blue << "plus_node::codegen() called" << norm << std::endl;
        Value *L = left->codegen(expCodeTab);
        Value *R = right->codegen(expCodeTab);
        if (!L || !R) return nullptr;
        return expCodeTab->builder.CreateAdd(L, R, "addtmp");
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

//        CallInst* int32_call = CallInst::Create(CalleeF, const_int32_13, "call", label_entry_18);
        return expCodeTab->builder.CreateCall(CalleeF, ArgsV, "calltmp");
    }

    Value *loop_node::codegen(ExpressionCodeTableP expCodeTab) {
        return nullptr;
    }

    Value *cond_node::codegen(ExpressionCodeTableP expCodeTab) {
        return nullptr;
    }

    std::vector<Value *> block_node::codegen_block(ExpressionCodeTableP expCodeTab) {
        expCodeTab->llvm_values_.enterscope();
        // do code gen
        std::vector<Value *> bodyCode;
        Value *V = nullptr;
        for (int i = body->first(); body->more(i); i = body->next(i))
        {
            V = body->nth(i)->codegen(expCodeTab);
            bodyCode.push_back(V);
        }
        expCodeTab->llvm_values_.exitscope();
        std::cout << red << "block node codegen output = "; V->dump(); std::cout << norm << std::endl;
        return bodyCode;
    }

    Value *block_node::codegen(ExpressionCodeTableP expCodeTab) {
        expCodeTab->llvm_values_.enterscope();
        // do code gen
        std::vector<Value *> bodyCode;
        Value *V = nullptr;
        for (int i = body->first(); body->more(i); i = body->next(i))
        {
            V = body->nth(i)->codegen(expCodeTab);
            bodyCode.push_back(V);
        }
        expCodeTab->llvm_values_.exitscope();
        return V;
    }

    Value *declaration_node::codegen(ExpressionCodeTableP expCodeTab) {
        Value *V = initialisation->codegen(expCodeTab);
        std::cout << "Adding decalaration for variable: " << name << std::endl;
        expCodeTab->llvm_values_.addid(name, V);
        return V;
    }

    void generate_string_var(ExpressionCodeTableP expCodeTab) {
//        typedef unsigned long long uint64_t;
//        uint64_t x = 14;
        std::cout << "generate_string_var() called" << std::endl;

        PointerType *PointerTy_6 = PointerType::get(IntegerType::get(expCodeTab->TheModule->getContext(), 8), 0);
        ConstantInt *const_int32_12 = ConstantInt::get(expCodeTab->TheModule->getContext(), APInt(32, StringRef("0"), 10));

        std::vector<Type *> FuncTy_8_args;
        FuncTy_8_args.emplace_back(PointerTy_6);
// ArrayRef<Type*> fty = ArrayRef(FuncTy_8_args);
        FunctionType *FuncTy_8 = FunctionType::get(
                /*Result=*/IntegerType::get(expCodeTab->TheModule->getContext(), 32),
                /*Params=*/FuncTy_8_args,
                /*isVarArg=*/true);

        std::vector<Type *> FuncTy_2_args;
        FuncTy_2_args.push_back(IntegerType::get(expCodeTab->TheModule->getContext(), 32));
        FunctionType *FuncTy_2 = FunctionType::get(
                /*Result=*/IntegerType::get(expCodeTab->TheModule->getContext(), 32),
                /*Params=*/FuncTy_2_args,
                /*isVarArg=*/false);

        Function *func_some_function = expCodeTab->TheModule->getFunction("some_function");
        if (!func_some_function) {
            func_some_function = Function::Create(
                    /*Type=*/FuncTy_2,
                    /*Linkage=*/GlobalValue::ExternalLinkage,
                    /*Name=*/"some_function", expCodeTab->TheModule.get());
            func_some_function->setCallingConv(CallingConv::C);
        }
        AttributeList func_some_function_PAL;
        {
            SmallVector<AttributeList, 4> Attrs;
            AttributeList PAS;
            {
                AttrBuilder B;
                B.addAttribute(Attribute::NoUnwind);
                B.addAttribute(Attribute::StackProtect);
                B.addAttribute(Attribute::UWTable);
                PAS = AttributeList::get(expCodeTab->TheModule->getContext(), ~0U, B);
            }

            Attrs.push_back(PAS);
            func_some_function_PAL = AttributeList::get(expCodeTab->TheModule->getContext(), Attrs);

        }
        func_some_function->setAttributes(func_some_function_PAL);

        Function *func_printf = expCodeTab->TheModule->getFunction("printf");
        if (!func_printf) {
            func_printf = Function::Create(
                    /*Type=*/FuncTy_8,
                    /*Linkage=*/GlobalValue::ExternalLinkage,
                    /*Name=*/"printf", expCodeTab->TheModule.get()); // (external, no body)
            func_printf->setCallingConv(CallingConv::C);
        }
        AttributeList func_printf_PAL;
        {
            SmallVector<AttributeList, 4> Attrs;
            AttributeList PAS;
            {
                AttrBuilder B;
                PAS = AttributeList::get(expCodeTab->TheModule->getContext(), ~0U, B);
            }

            Attrs.push_back(PAS);
            func_printf_PAL = AttributeList::get(expCodeTab->TheModule->getContext(), Attrs);

        }
        func_printf->setAttributes(func_printf_PAL);


        std::vector<Type *> FuncTy_4_args;
        FunctionType *FuncTy_4 = FunctionType::get(
                /*Result=*/IntegerType::get(expCodeTab->TheModule->getContext(), 32),
                /*Params=*/FuncTy_4_args,
                /*isVarArg=*/false);

        Function *func_main = expCodeTab->TheModule->getFunction("main");
        if (!func_main) {
            func_main = Function::Create(
                    /*Type=*/FuncTy_4,
                    /*Linkage=*/GlobalValue::ExternalLinkage,
                    /*Name=*/"main", expCodeTab->TheModule.get());
            func_main->setCallingConv(CallingConv::C);
        }
        AttributeList func_main_PAL;
        {
            SmallVector<AttributeList, 4> Attrs;
            AttributeList PAS;
            {
                AttrBuilder B;
                B.addAttribute(Attribute::NoUnwind);
                B.addAttribute(Attribute::StackProtect);
                B.addAttribute(Attribute::UWTable);
                PAS = AttributeList::get(expCodeTab->TheModule->getContext(), ~0U, B);
            }

            Attrs.push_back(PAS);
            func_main_PAL = AttributeList::get(expCodeTab->TheModule->getContext(), Attrs);

        }
        func_main->setAttributes(func_main_PAL);


        ArrayType* ArrayTy_0 = ArrayType::get(IntegerType::get(expCodeTab->TheModule->getContext(), 8), 13);
        Constant *const_array_4 = ConstantDataArray::getString(expCodeTab->TheModule->getContext(), "hello world\x0A", true);
//        Constant *const_array_4 = ConstantDataArray::getString(expCodeTab->TheModule->getContext(), "hello world");
        const_array_4->getType()->dump();
        ArrayTy_0->dump();
        GlobalVariable *gvar_array__str = new GlobalVariable(
                *(expCodeTab->TheModule),
                ArrayTy_0,
                true,
                GlobalValue::PrivateLinkage,
                0, // has initializer, specified below
                ".str");
        gvar_array__str->setAlignment(1);


        gvar_array__str->setInitializer(const_array_4);


//        ConstantInt *const_int32_12 = ConstantInt::get(expCodeTab->TheModule->getContext(), APInt(32, StringRef("0"), 10));



        std::vector<Constant *> const_ptr_5_indices;
        ConstantInt* const_int64_6 = ConstantInt::get(TheContext, APInt(32,0, false));
        Constant *const_ptr_5 = ConstantExpr::getGetElementPtr(ArrayTy_0, gvar_array__str, const_ptr_5_indices);
        ConstantInt *const_int32_13 = ConstantInt::get(expCodeTab->TheModule->getContext(), APInt(32, StringRef("21"), 10));


        const_ptr_5_indices.emplace_back( const_ptr_5 );
        const_ptr_5_indices.emplace_back( const_int64_6);



        BasicBlock *label_entry_18 = BasicBlock::Create(expCodeTab->TheModule->getContext(), "entry", func_main, 0);

        AllocaInst *ptr_retval = new AllocaInst(IntegerType::get(expCodeTab->TheModule->getContext(), 32), "retval", label_entry_18);
        ptr_retval->setAlignment(4);
        AllocaInst *ptr_answer = new AllocaInst(IntegerType::get(expCodeTab->TheModule->getContext(), 32), "answer", label_entry_18);
        ptr_answer->setAlignment(4);
        StoreInst *void_19 = new StoreInst(const_int32_12, ptr_retval, false, label_entry_18);
//        CallInst *int32_call = CallInst::Create(func_some_function, const_int32_13, "call", label_entry_18);
//        int32_call->setCallingConv(CallingConv::C);
//        int32_call->setTailCall(false);
//        AttributeList int32_call_PAL;
//        int32_call->setAttributes(int32_call_PAL);

        CallInst *int32_call = CallInst::Create(func_some_function, const_int32_13, "call", label_entry_18);
        int32_call->setCallingConv(CallingConv::C);
        int32_call->setTailCall(false);
        AttributeList int32_call_PAL;
        int32_call->setAttributes(int32_call_PAL);

        StoreInst *void_20 = new StoreInst(int32_call, ptr_answer, false, label_entry_18);

        void_20->setAlignment(4);
        LoadInst *int32_21 = new LoadInst(ptr_answer, "", false, label_entry_18);
        int32_21->setAlignment(4);
        std::vector<Value *> int32_call1_params;
        int32_call1_params.emplace_back(const_ptr_5);
        int32_call1_params.emplace_back(int32_21);
//        CallInst *int32_call1 = CallInst::Create(func_printf, int32_call1_params, "call1", label_entry_18);
//        int32_call1->setCallingConv(CallingConv::C);
//        int32_call1->setTailCall(false);
//        AttributeList int32_call1_PAL;
//        int32_call1->setAttributes(int32_call1_PAL);

        ReturnInst::Create(expCodeTab->TheModule->getContext(), const_int64_6, label_entry_18);



//        BasicBlock *label_entry_18 = BasicBlock::Create(expCodeTab->TheModule->getContext(), "entry", func_main, 0);
//        LoadInst *int32_21 = new LoadInst(const_int64_6, "", false, label_entry_18);
//        int32_21->setAlignment(4);
//        CallInst *cI = CallInst::Create(expCodeTab->CalleeF, const_ptr_5_indices, "call1", label_entry_18);

//        CallInst *cI = expCodeTab->builder.CreateCall(expCodeTab->CalleeF, const_ptr_5_indices, "calling", label_entry_18);

//        CallInst *int32_call1 = CallInst::Create(func_printf, int32_call1_params, "call1", label_entry_18);
//        cI->setCallingConv(CallingConv::C);
//        cI->setTailCall(false);
//        AttributeList int32_call1_PAL;
//        cI->setAttributes(int32_call1_PAL);
//
//
//        cI->dump();
//
//        ReturnInst::Create(expCodeTab->TheModule->getContext(), const_int64_6, label_entry_18);

    }

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

        Value * RetVal = body->codegen(expCodeTab);

        if (RetVal) {
            // Finish off the function.
//            expCodeTab->builder.
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
        return function_codegen(expCodeTab);
//        return nullptr;
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