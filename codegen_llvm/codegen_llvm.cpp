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
        expCodeTab->CalleeF = expCodeTab->TheModule->getOrInsertFunction("printf",
                                 FunctionType::get(
                                     IntegerType::getInt32Ty(TheContext),
                                     PointerType::get(Type::getInt8Ty(TheContext), 0),
                                     true /* this is var arg func type*/)
        );
        expCodeTab->llvm_values_.enterscope();
        exps->codegen(expCodeTab);
        expCodeTab->llvm_values_.exitscope();
        // Print out all of the generated code.
        expCodeTab->TheModule->print(errs(), nullptr);
        expCodeTab->TheModule->dump();


        expCodeTab->generateObjectCode();
        std::cout << "Object code generation complete" << std::endl;
    }

    /// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
    /// the function.  This is used for mutable variables etc.
    static AllocaInst *CreateEntryBlockAlloca(Function *TheFunction,
                                              const std::string &VarName, Type *VarType) {
        IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                         TheFunction->getEntryBlock().begin());
        return TmpB.CreateAlloca(VarType, nullptr, VarName);
    }

    /// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
    /// the function.  This is used for mutable variables etc.
    static AllocaInst *CreateGlobalAlloca( ExpressionCodeTableP expCodeTab,
                                              const std::string &VarName, Type *VarType) {
        IRBuilder<> TmpB(expCodeTab->TheModule->getContext());
        return TmpB.CreateAlloca(VarType, nullptr, VarName);
    }




    int ExpressionCodeTable::generateObjectCode() {
        std::cout << "generateObjectCode() called" << std::endl;


        Mod = makeLLVMModule(this);
        PassManager<Module> *PM = new PassManager<Module>();
        AnalysisManager<Module> AM;
        std::cout << "executing main()... print called on module:" << std::endl;
        std::cout << "executing main()... dump caled on module:" << std::endl;
        Mod->dump();


        InitializeAllTargetInfos();
        InitializeAllTargets();
        InitializeAllTargetMCs();
        InitializeAllAsmParsers();
        InitializeAllAsmPrinters();

        auto TargetTriple = sys::getDefaultTargetTriple();
        Mod->setTargetTriple(TargetTriple);

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

        Mod->setDataLayout(TheTargetMachine->createDataLayout());

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


        pass.run(*Mod);
        dest.flush();

        outs() << "Wrote " << Filename << "\n";

        return 0;

    }





    Value *integer_node::codegen(ExpressionCodeTableP expCodeTab) {
        std::cout << "integer node codegen called" << std::endl;
        return ConstantInt::get(Type::getInt32Ty(TheContext), StringRef(sym->get_string()), (u_int8_t) 10);
    }

    Value *minus_node::codegen(ExpressionCodeTableP expCodeTab) {
        Value *L = left->codegen(expCodeTab);
        Value *R = right->codegen(expCodeTab);
        if (!L || !R) return nullptr;
        return expCodeTab->builder.CreateFSub(L, R, "subtmp");
    }

    Value *unary_minus_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    static Type *inferType(ExpressionCodeTableP expCodeTab, Expression exp) {

        std::cout <<  "inferType(), type = " << std::endl;

        RwlTypes rwlType = TypeCheck::checkType(exp->get_type());

        switch (TypeCheck::checkType(exp->get_type())) {
            case RwlTypes::STRING: {

                std::string strValue = exp->get_value()->get_string();
                return ArrayType::get(IntegerType::get(expCodeTab->TheModule->getContext(), 8), strValue.length() + 1);

            }

            case RwlTypes::INT: {
                return IntegerType::get(expCodeTab->TheModule->getContext(), 32);

            }

        }
    }

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
        std::cout << "id_node::codegen() called" << std::endl;
        Value *V;
        if (!(V = expCodeTab->NamedValues[sym->get_string().c_str()]) )
            V = expCodeTab->llvm_values_.lookup(sym);
        return expCodeTab->builder.CreateLoad(V, sym->get_string().c_str());
//        return V;
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
        Value *CondV = predicate->codegen(expCodeTab);
        if (!CondV)
            return nullptr;

        // Convert condition to a bool by comparing non-equal to 0.0.
        CondV = expCodeTab->builder.CreateICmpNE(
                CondV, ConstantInt::get(TheContext, APInt(32, 0)), "ifcond");

        Function *TheFunction = expCodeTab->builder.GetInsertBlock()->getParent();

        // Create blocks for the then and else cases.  Insert the 'then' block at the
        // end of the function.
        BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
        BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else");
        BasicBlock *MergeBB = BasicBlock::Create(TheContext, "ifcont");

        expCodeTab->builder.CreateCondBr(CondV, ThenBB, ElseBB);

        // Emit then value.
        expCodeTab->builder.SetInsertPoint(ThenBB);

        Value *ThenV = if_body->codegen(expCodeTab);
        if (!ThenV)
            return nullptr;

        expCodeTab->builder.CreateBr(MergeBB);
        // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
        ThenBB = expCodeTab->builder.GetInsertBlock();

        // Emit else block.
        TheFunction->getBasicBlockList().push_back(ElseBB);
        expCodeTab->builder.SetInsertPoint(ElseBB);

        Value *ElseV = else_body->codegen(expCodeTab);
        if (!ElseV)
            return nullptr;

        expCodeTab->builder.CreateBr(MergeBB);
        // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
        ElseBB = expCodeTab->builder.GetInsertBlock();

        // Emit merge block.
        TheFunction->getBasicBlockList().push_back(MergeBB);
        expCodeTab->builder.SetInsertPoint(MergeBB);
        PHINode *PN = expCodeTab->builder.CreatePHI(Type::getInt32Ty(TheContext), 10, "iftmp");

        PN->addIncoming(ThenV, ThenBB);
        PN->addIncoming(ElseV, ElseBB);
        return PN;
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
        AllocaInst *Alloca = nullptr;
        if (getIsInsideFunction())
            Alloca = CreateGlobalAlloca(expCodeTab, name->get_string(), inferType(expCodeTab, initialisation));
        else
            Alloca = CreateEntryBlockAlloca(expCodeTab->builder.GetInsertBlock()->getParent(), name->get_string(), inferType(expCodeTab, initialisation));
        std::cout << "Adding decalaration for variable: " << name << std::endl;
        expCodeTab->llvm_values_.addid(name, V);
        expCodeTab->builder.CreateStore(expCodeTab->llvm_values_.lookup(name), Alloca);
        return V;
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
        for (auto &Arg : TheFunction->args()) {
            // Create an alloca for this variable.
            AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, Arg.getName(), Arg.getType());

            // Store the initial value into the alloca.
            expCodeTab->builder.CreateStore(&Arg, Alloca);

            // Add arguments to variable symbol table.
            // expCodeTab->llvm_values_.addid(, Alloca);
            expCodeTab->NamedValues[Arg.getName()] = Alloca;
        }

//         Record the function arguments in the NamedValues map.
//        expCodeTab->NamedValues.clear();
//        for (auto &Arg : TheFunction->args()) {
//            expCodeTab->NamedValues[Arg.getName()] = &Arg;
//            std::cout << "Funtion " << name->get_string() << ", parameter type: "; Arg.dump(); std::cout << ", parameter name: " << Arg.getName().data() << std::endl;
//        }



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
    }



    Value *assignment_stmt::codegen(ExpressionCodeTableP expCodeTab) {
        Value *Var = ( expCodeTab->llvm_values_.lookup(_id->get_symbol()) );
        if (!Var) {
            std::cerr << red <<  "Unknown variable name: " << norm << _id->get_string() << std::endl;
            return nullptr;
        }

        Value *V = exp->codegen(expCodeTab);

        expCodeTab->builder.CreateStore(V, Var);

        return V;
    }

    Value *operator_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    Value *formal_node::codegen(ExpressionCodeTableP expCodeTab) { return nullptr; }

    Module *makeLLVMModule(ExpressionCodeTableP expCodeTab) {
        // Module Construction
        Module *mod = expCodeTab->TheModule.get();
        mod->setDataLayout(
                "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128");
        mod->setTargetTriple("x86_64-apple-macosx10.10.0");

        // Type Definitions
        ArrayType *ArrayTy_0 = ArrayType::get(IntegerType::get(mod->getContext(), 8), 14);

        PointerType *PointerTy_1 = PointerType::get(ArrayTy_0, 0);

        std::vector<Type *> FuncTy_2_args;
        FuncTy_2_args.push_back(IntegerType::get(mod->getContext(), 32));
        FunctionType *FuncTy_2 = FunctionType::get(
                /*Result=*/IntegerType::get(mod->getContext(), 32),
                /*Params=*/FuncTy_2_args,
                /*isVarArg=*/false);

        PointerType *PointerTy_3 = PointerType::get(IntegerType::get(mod->getContext(), 32), 0);

        std::vector<Type *> FuncTy_4_args;
        FunctionType *FuncTy_4 = FunctionType::get(
                /*Result=*/IntegerType::get(mod->getContext(), 32),
                /*Params=*/FuncTy_4_args,
                /*isVarArg=*/false);

        PointerType *PointerTy_5 = PointerType::get(FuncTy_2, 0);

        PointerType *PointerTy_6 = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);

        std::vector<Type *> FuncTy_8_args;
        FuncTy_8_args.emplace_back(PointerTy_6);
        FunctionType *FuncTy_8 = FunctionType::get(
                /*Result=*/IntegerType::get(mod->getContext(), 32),
                /*Params=*/FuncTy_8_args,
                /*isVarArg=*/true);

        PointerType *PointerTy_7 = PointerType::get(FuncTy_8, 0);


        // Function Declarations

        Function *func_new_function = mod->getFunction("new_fn");

        Function *func_some_function = mod->getFunction("some_function");
        if (!func_some_function) {
            func_some_function = Function::Create(
                    /*Type=*/FuncTy_2,
                    /*Linkage=*/GlobalValue::ExternalLinkage,
                    /*Name=*/"some_function", mod);
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
                PAS = AttributeList::get(mod->getContext(), ~0U, B);
            }

            Attrs.push_back(PAS);
            func_some_function_PAL = AttributeList::get(mod->getContext(), Attrs);

        }
        func_some_function->setAttributes(func_some_function_PAL);

        Function *func_main = mod->getFunction("main");
        if (!func_main) {
            func_main = Function::Create(
                    /*Type=*/FuncTy_4,
                    /*Linkage=*/GlobalValue::ExternalLinkage,
                    /*Name=*/"main", mod);
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
                PAS = AttributeList::get(mod->getContext(), ~0U, B);
            }

            Attrs.push_back(PAS);
            func_main_PAL = AttributeList::get(mod->getContext(), Attrs);

        }
        func_main->setAttributes(func_main_PAL);

        Function *func_printf = mod->getFunction("printf");
        if (!func_printf) {
            func_printf = Function::Create(
                    /*Type=*/FuncTy_8,
                    /*Linkage=*/GlobalValue::ExternalLinkage,
                    /*Name=*/"printf", mod); // (external, no body)
            func_printf->setCallingConv(CallingConv::C);
        }
        AttributeList func_printf_PAL;
        {
            SmallVector<AttributeList, 4> Attrs;
            AttributeList PAS;
            {
                AttrBuilder B;
                PAS = AttributeList::get(mod->getContext(), ~0U, B);
            }

            Attrs.push_back(PAS);
            func_printf_PAL = AttributeList::get(mod->getContext(), Attrs);

        }
        func_printf->setAttributes(func_printf_PAL);

        // Global Variable Declarations


        GlobalVariable *gvar_array__str = new GlobalVariable(/*Module=*/*mod,
                /*Type=*/ArrayTy_0,
                /*isConstant=*/true,
                /*Linkage=*/GlobalValue::PrivateLinkage,
                /*Initializer=*/0, // has initializer, specified below
                /*Name=*/".str");
        gvar_array__str->setAlignment(1);

        // Constant Definitions
        Constant *const_array_9 = ConstantDataArray::getString(mod->getContext(), "Answer is %d\x0A", true);
        ConstantInt *const_int32_10 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("1"), 10));
        ConstantInt *const_int32_11 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("2"), 10));
        ConstantInt *const_int32_12 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("0"), 10));
        ConstantInt *const_int32_13 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("21"), 10));
        std::vector<Constant *> const_ptr_14_indices;
        const_ptr_14_indices.emplace_back(const_int32_12);
        const_ptr_14_indices.emplace_back(const_int32_12);
        Constant *const_ptr_14 = ConstantExpr::getGetElementPtr(ArrayTy_0, gvar_array__str, const_ptr_14_indices);

        // Global Variable Definitions
        gvar_array__str->setInitializer(const_array_9);

        // Function Definitions

        // Function: some_function (func_some_function)
        {
            Function::arg_iterator args = func_some_function->arg_begin();
            Value *int32_input = args++;
            int32_input->setName("input");

            BasicBlock *label_entry = BasicBlock::Create(mod->getContext(), "entry", func_some_function, 0);

            // Block entry (label_entry)
            AllocaInst *ptr_input_addr = new AllocaInst(IntegerType::get(mod->getContext(), 32), "input.addr", label_entry);
            ptr_input_addr->setAlignment(4);
            StoreInst *void_15 = new StoreInst(int32_input, ptr_input_addr, false, label_entry);
            void_15->setAlignment(4);
            LoadInst *int32_16 = new LoadInst(ptr_input_addr, "", false, label_entry);
            int32_16->setAlignment(4);
            BinaryOperator *int32_mul = BinaryOperator::Create(Instruction::Mul, int32_16, const_int32_11, "mul",
                                                               label_entry);
            ReturnInst::Create(mod->getContext(), int32_mul, label_entry);

        }

        // Function: main (func_main)
        {

            BasicBlock *label_entry_18 = BasicBlock::Create(mod->getContext(), "entry", func_main, 0);

            // Block entry (label_entry_18)
            AllocaInst *ptr_retval = new AllocaInst(IntegerType::get(mod->getContext(), 32), "retval", label_entry_18);
            ptr_retval->setAlignment(4);
            AllocaInst *ptr_answer = new AllocaInst(IntegerType::get(mod->getContext(), 32), "answer", label_entry_18);
            ptr_answer->setAlignment(4);
            StoreInst *void_19 = new StoreInst(const_int32_12, ptr_retval, false, label_entry_18);
            CallInst *int32_call = CallInst::Create(func_new_function, const_int32_13, "call", label_entry_18);
            int32_call->setCallingConv(CallingConv::C);
            int32_call->setTailCall(false);
            AttributeList int32_call_PAL;
            int32_call->setAttributes(int32_call_PAL);

            StoreInst *void_20 = new StoreInst(int32_call, ptr_answer, false, label_entry_18);
            void_20->setAlignment(4);
            LoadInst *int32_21 = new LoadInst(ptr_answer, "", false, label_entry_18);
            int32_21->setAlignment(4);
            std::vector<Value *> int32_call1_params;
            int32_call1_params.emplace_back(const_ptr_14);
            int32_call1_params.emplace_back(int32_21);
            CallInst *int32_call1 = CallInst::Create(func_printf, int32_call1_params, "call1", label_entry_18);
            int32_call1->setCallingConv(CallingConv::C);
            int32_call1->setTailCall(false);
            AttributeList int32_call1_PAL;
            int32_call1->setAttributes(int32_call1_PAL);

            ReturnInst::Create(mod->getContext(), const_int32_12, label_entry_18);

        }

        return mod;
    }

}