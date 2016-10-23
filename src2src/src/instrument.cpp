//------------------------------------------------------------------------------
// Tooling sample. Demonstrates:
//
// * How to write a simple source tool using libTooling.
// * How to use RecursiveASTVisitor to find interesting AST nodes.
// * How to use the Rewriter API to rewrite the source code.
//
// Eli Bendersky (eliben@gmail.com)
// This code is in the public domain
//------------------------------------------------------------------------------
#include <sstream>
#include <string>
#include <vector>
#include <tuple>

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;

static llvm::cl::OptionCategory ToolingSampleCategory("Tooling Sample");

class RemoveCrcCallsVisitor : public RecursiveASTVisitor<RemoveCrcCallsVisitor> {
public:
    RemoveCrcCallsVisitor(Rewriter &R, const std::vector<std::tuple<std::string,uint64_t, std::string>> &vars):
                                TheRewriter(R),
                                vars_to_remove(vars) {}
    bool VisitStmt(Stmt *s) {
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
private:
      Rewriter &TheRewriter;
      const std::vector<std::tuple<std::string,uint64_t, std::string>> &vars_to_remove;

 
};

// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor> {
public:
  std::vector<std::tuple<std::string,uint64_t, std::string>> global_vars;
  MyASTVisitor(Rewriter &R, ASTContext *C) : TheRewriter(R), Context(C) {}
  
  bool VisitVarDecl(VarDecl *decl) {
      if(decl->hasGlobalStorage() || decl->isStaticDataMember() ) {
          const Type *type = decl->getType().getTypePtr();
          if(!decl->hasInit()) return true;
          if(!type->isIntegerType()) return true;
          if(type->isUnsignedIntegerType()) return true;
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

  bool VisitFunctionDecl(FunctionDecl *f) {
    // Only function definitions (with bodies), not declarations.
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
            SSsymbolize << "\tklee_make_symbolic( &" << var_name << ", " 
                                                   << size << ", \"" 
                                                   << var_name << "\");\n";
            SSsymbolize << "\tif( " << var_name << " <  " <<  var_value 
                        << " || " << var_name << " >  " <<  var_value 
                        << " ) klee_silent_exit(0); \n\n";


            printSymbolic << "klee_print_symbolic(\""<< var_name  << "\", &" << var_name << ", " << size*8 << "); \n"; 
          }

          TheRewriter.InsertText(FuncBody->getLocStart().getLocWithOffset(1), SSsymbolize.str(), true, true);
          TheRewriter.InsertText(FuncBody->getLocEnd().getLocWithOffset(-12), printSymbolic.str(), true, true);
         RemoveCrcCallsVisitor remover(TheRewriter, global_vars);
         remover.TraverseStmt(FuncBody);
    
      }
    }

    return true;
  }

private:
  Rewriter &TheRewriter;
  ASTContext *Context;
};


class FunctionLogger : public RecursiveASTVisitor<FunctionLogger> {
public:
  FunctionLogger(Rewriter &R, ASTContext *C) : TheRewriter(R), Context(C) {}
  
    bool VisitFunctionDecl(FunctionDecl *f) {
    // Only function definitions (with bodies), not declarations.
        if (f->hasBody()) {
            Stmt *FuncBody = f->getBody();
            // Function name
            DeclarationName DeclName = f->getNameInfo().getName();
            std::string FuncName = DeclName.getAsString();
            std::stringstream logFunction;
            logFunction << "\n logFunction(\"" <<  FuncName << "\");\n";
            TheRewriter.InsertText(FuncBody->getLocStart().getLocWithOffset(1), logFunction.str(), true, true);
        }
        return true;
  }

private:
  Rewriter &TheRewriter;
  ASTContext *Context;
};



// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class MyASTConsumer : public ASTConsumer {
public:
  MyASTConsumer(Rewriter &R, ASTContext *C) : Visitor(R, C), functionCallsInstrument(R, C) {}

  // Override the method that gets called for each parsed top-level
  // declaration.
  bool HandleTopLevelDecl(DeclGroupRef DR) override {
    for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b) {
      // Traverse the declaration using our AST visitor.
      Visitor.TraverseDecl(*b);
      functionCallsInstrument.TraverseDecl(*b);

      (*b)->dump();
    }
    return true;
  }

private:
  MyASTVisitor Visitor;
  FunctionLogger functionCallsInstrument;
};

// For each source file provided to the tool, a new FrontendAction is created.
class MyFrontendAction : public ASTFrontendAction {
public:
  MyFrontendAction() {}
  void EndSourceFileAction() override {
    SourceManager &SM = TheRewriter.getSourceMgr();
    const char* fileName = SM.getFileEntryForID(SM.getMainFileID())->getName(); 
    std::stringstream fileLocation;
    fileLocation << "\nchar*  __klee__instr_filename = \"" << basename(fileName) << ".out\";\n";
    
    TheRewriter.InsertText(SM.getLocForStartOfFile(SM.getMainFileID()), fileLocation.str(), true, true);
    
    
    llvm::errs() << "** EndSourceFileAction for: "
                 << fileName << "\n";

    // Now emit the rewritten buffer.
    TheRewriter.getEditBuffer(SM.getMainFileID()).write(llvm::outs());
  }

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) override {
    llvm::errs() << "** Creating AST consumer for: " << file << "\n";
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return llvm::make_unique<MyASTConsumer>(TheRewriter, &CI.getASTContext());
  }

private:
  Rewriter TheRewriter;
};

int main(int argc, const char **argv) {
  CommonOptionsParser op(argc, argv, ToolingSampleCategory);
  ClangTool Tool(op.getCompilations(), op.getSourcePathList());

  // ClangTool::run accepts a FrontendActionFactory, which is then used to
  // create new objects implementing the FrontendAction interface. Here we use
  // the helper newFrontendActionFactory to create a default factory that will
  // return a new MyFrontendAction object every time.
  // To further customize this, we could create our own factory class.
  return Tool.run(newFrontendActionFactory<MyFrontendAction>().get());
}
