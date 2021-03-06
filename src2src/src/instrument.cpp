//-----------------------------------------------------------------------------
// *Adds logFunction() call at the start of every function.
// *Symbolizes integers
// *Removes calls to transparent_crc for those integers
//------------------------------------------------------------------------------
#include <sstream>
#include <string>

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
#include "llvm/Support/CommandLine.h"

#include "FunctionLogger.hpp"
#include "ToSSATransformer.hpp"
#include "DeadConditionTransformer.hpp"
#include "SwapBranchesTransformer.hpp"
#include "Symbolizer.hpp"
#include "RemoveFunction.hpp"

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;

static llvm::cl::OptionCategory ToolingSampleCategory("Semantic perserving transformations");
static llvm::cl::opt<bool> toSSA ("toSSA", llvm::cl::desc("Perform to-SSA-like transform instead"),
                                           llvm::cl::cat(ToolingSampleCategory));
static llvm::cl::opt<bool> deadCond ("deadCond", llvm::cl::desc("Perform dead condition injection instead"),
                                           llvm::cl::cat(ToolingSampleCategory));

static llvm::cl::opt<bool> swapBranch ("swapBranches", llvm::cl::desc("Swap if and else branch order"),
                                           llvm::cl::cat(ToolingSampleCategory));
static llvm::cl::opt<bool> removeMain("r", llvm::cl::desc("Remove  function main"), 
                                                 llvm::cl::cat(ToolingSampleCategory));


class MyASTConsumer : public ASTConsumer {
public:
  MyASTConsumer(Rewriter &R, ASTContext *C) : Visitor(R, C),
                                              toSSATransformer(R,C), 
                                              functionCallsInstrument(R, C),
                                              ctx(C),
                                              rw(R) {}
    virtual void HandleTranslationUnit(clang::ASTContext &Context) {
      if(toSSA) {
        toSSATransformer.TraverseDecl(Context.getTranslationUnitDecl());
      } else if (swapBranch) {
        swapBranches( ctx, rw);
      } else if(removeMain) {
        removeFunction("main", ctx, rw);
      } else if (deadCond) {
        llvm::errs() << "handle top level ============== other stuff\n";
        injectDeadConditions( ctx, rw);
      } else{
         llvm::errs() << "other stuff\n";
         Visitor.TraverseDecl(Context.getTranslationUnitDecl());
         functionCallsInstrument.TraverseDecl(Context.getTranslationUnitDecl());
      }
  }

private:
  SymbolizeIntegers Visitor;
  ToSSATransformer toSSATransformer;
  FunctionLogger functionCallsInstrument;
  ASTContext *ctx;
  Rewriter &rw;

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
    
    if(!toSSA && !deadCond && !swapBranch && !removeMain)
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
