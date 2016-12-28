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
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/RefactoringCallbacks.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"

#include "FunctionLogger.hpp"
#include "ToSSATransformer.hpp"
#include "Symbolizer.hpp"

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace clang::ast_matchers;

static llvm::cl::OptionCategory ToolingSampleCategory("Tooling Sample");
static llvm::cl::opt<bool> toSSA ("toSSA", llvm::cl::desc("Perform to-SSA-like transform instead"),
                                           llvm::cl::cat(ToolingSampleCategory));

class MyASTConsumer : public ASTConsumer {
public:
  MyASTConsumer(Rewriter &R, ASTContext *C) : Visitor(R, C),
                                              toSSATransformer(R,C), 
                                              functionCallsInstrument(R, C),
                                              ctx(C),
                                              rw(R) {}

  // Override the method that gets called for each parsed top-level

  // declaration.
  bool HandleTopLevelDecl(DeclGroupRef DR) override {
    for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b) {
      // Traverse the declaration using our AST visitor.
      if(toSSA) {
         MatchFinder Finder;
         IfStmtHandler ih(rw, ctx);

         //Finder.addMatcher(ifStmt().bind("ifStmt"), &ih);
         Finder.addMatcher(ifStmt(forEach(declRefExpr().bind("refExpr"))).bind("ifStmt"), &ih);
//         Finder.matchAST(*ctx);
//         ReplaceStmtWithText Callback("integer", "42");
//         StatementMatcher sm = integerLiteral().bind("integer");
//         Finder.addMatcher(sm, &Callback);
//         llvm::errs() << "TO ssa\n";
         toSSATransformer.TraverseDecl(*b);
        

      } else{
         llvm::errs() << "other stuff\n";
         Visitor.TraverseDecl(*b);
         functionCallsInstrument.TraverseDecl(*b);
      }
           (*b)->dump();
    }
    return true;
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

StatementMatcher LoopMatcher =
  forStmt(hasLoopInit(declStmt(hasSingleDecl(varDecl(
    hasInitializer(integerLiteral(equals(0)))))))).bind("forLoop");

class LoopPrinter : public MatchFinder::MatchCallback {
public :
  virtual void run(const MatchFinder::MatchResult &Result) {
    if (const ForStmt *FS = Result.Nodes.getNodeAs<clang::ForStmt>("forLoop"))
      FS->dump();
  }
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
