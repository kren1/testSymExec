#include "Symbolizer.hpp"

using namespace clang;

RemoveCrcCallsVisitor::RemoveCrcCallsVisitor(
        Rewriter &R, 
        const std::vector<std::tuple<std::string,uint64_t, std::string>> &vars
    ): TheRewriter(R), vars_to_remove(vars) {}

bool RemoveCrcCallsVisitor::VisitStmt(Stmt *s) {
    if(isa<CallExpr>(s)) {
        CallExpr *callStmt = cast<CallExpr>(s);
        std::string name = callStmt->getDirectCallee()->getNameAsString();
        llvm::errs() << " ==== Visited Call Stmt " ;
        llvm::errs() << name ;
        if(name == "transparent_crc") {
            const Expr *secondArg = callStmt->getArg(1)->IgnoreImplicit();
            llvm::errs() << isa<StringLiteral>(secondArg);
            const StringLiteral *var_name = cast<StringLiteral>(secondArg);
            std::string name = var_name->getString().str();
            llvm::errs() << " - " << name;
            //if second argument is in vars_to_remove)
            if(std::find_if(vars_to_remove.begin(), 
                            vars_to_remove.end(), 
                            [&name](const std::tuple<std::string,uint64_t, std::string> &tuple) 
                                        { return std::get<0>(tuple) == name; } ) != vars_to_remove.end() ) {
                llvm::errs() << " --- HIT, REMOVING !!!!";
                TheRewriter.InsertText(callStmt->getLocStart(), "/* ", true, true);
                TheRewriter.InsertText(callStmt->getLocEnd().getLocWithOffset(2), " */", true, true);
            }

        }
        llvm::errs() << "\n" ;

    }
    return true;
}

SymbolizeIntegers::SymbolizeIntegers(Rewriter &R, ASTContext *C) : TheRewriter(R), Context(C) {}
bool SymbolizeIntegers::VisitVarDecl(VarDecl *decl) {
    if(decl->hasGlobalStorage() || decl->isStaticDataMember() ) {
        const Type *type = decl->getType().getTypePtr();
        if(!decl->hasInit()) return true;
        if(!type->isIntegerType()) return true;
        if(!type->isUnsignedIntegerType()) return true;
        if(decl->getType().isConstQualified()) return true;

        uint64_t size = Context->getTypeInfo(decl->getType()).Width;

        llvm::errs() << " GLOBAL VAR:  " 
                     << decl->getDeclName().getAsString()  << " "
                     << size << " " << decl->getType().getAsString()  << " ";
        APValue *val =  decl->evaluateValue();
        if(val != NULL) {
           llvm::errs() << val->getAsString(*Context, decl->getType());
           std::string var_name = decl->getDeclName().getAsString(); 
           if(var_name.find("crc32") == std::string::npos) {
              global_vars.push_back(std::make_tuple(var_name, size,val->getAsString(*Context, decl->getType())));
           }
        }
        else
          llvm::errs() << " NULLLL ";

        llvm::errs() << " END NAME \n";
      }
      return true;
}

bool SymbolizeIntegers::VisitFunctionDecl(FunctionDecl *f) {
    if (f->hasBody()) {
      Stmt *FuncBody = f->getBody();
      // Function name
      DeclarationName DeclName = f->getNameInfo().getName();
      std::string FuncName = DeclName.getAsString();
      if(FuncName == "main") {
          llvm::errs() << "GLOBAL in main";
          std::stringstream SSsymbolize;
          std::stringstream printSymbolic;

          SSsymbolize << "\n // ====== symbolize and constrain ===== \n\n";
          for(auto i = global_vars.begin(); i != global_vars.end(); i++) {
            std::string var_name = std::get<0>(*i);
            uint64_t size  = std::get<1>(*i) / 8;
            std::string var_value = std::get<2>(*i);
            llvm::errs() << "GLOBAL vars main " << var_name << " " 
                                                << size << " " 
                                                 << var_value << " " << "\n"
                                               ;
            SSsymbolize << "\tsymbolize_and_constrain_u( &" << var_name << ", " 
                                                   << size << ", " 
                                                   << var_value  << ", \"" 
                                                   << var_name << "\");\n";
 
            printSymbolic << "print_symbolic(\""<< var_name  << "\", &" << var_name << ", " << size*8 << "); \n";
          }

          TheRewriter.InsertText(FuncBody->getLocStart().getLocWithOffset(1), SSsymbolize.str(), true, true);
          TheRewriter.InsertText(FuncBody->getLocEnd().getLocWithOffset(-12), printSymbolic.str(), true, true);
         RemoveCrcCallsVisitor remover(TheRewriter, global_vars);
         remover.TraverseStmt(FuncBody);
      }
    }
    return true;
}
