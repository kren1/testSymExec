#include "DeadConditionTransformer.hpp"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <set>
#include <string>

using namespace clang::ast_matchers;

class BranchSwapper : public MatchFinder::MatchCallback {
public:
  BranchSwapper(Rewriter &Rewrite) : Rewrite(Rewrite) {}

  virtual void run(const MatchFinder::MatchResult &Result) {
    // The matched 'if' statement was bound to 'ifStmt'.
    if (const IfStmt *IfS = Result.Nodes.getNodeAs<IfStmt>("ifStmt")) {
      clang::LangOptions LangOpts;
      clang::PrintingPolicy Policy(LangOpts);

      llvm::errs() << "Found if\n==================================\n";
      if(IfS->getThen() == NULL || IfS->getElse() == NULL) return;
      if(visitedIfs.count(IfS) > 0) {
        llvm::errs() << "Found duplciate: " << IfS << "\n";
        return;
      }

      std::string then;
      std::string els;
      
      llvm::raw_string_ostream::raw_string_ostream ss(then);
      llvm::raw_string_ostream::raw_string_ostream sse(els);
//      IfS->getThen() //store somehow
      IfS->getThen()->printPretty(ss,NULL,Policy,0 );
      IfS->getElse()->printPretty(sse,NULL,Policy,0 );

      Rewrite.ReplaceText(IfS->getThen()->getSourceRange(), StringRef(sse.str()));
      Rewrite.ReplaceText(IfS->getElse()->getSourceRange(), StringRef(ss.str()) );

      Rewrite.InsertText(IfS->getCond()->getLocStart(), "!(");
      Rewrite.InsertText(IfS->getCond()->getLocEnd().getLocWithOffset(1), ")");
      visitedIfs.insert(IfS);
      
    }
  }

private:
  Rewriter &Rewrite;
  std::set<const IfStmt*> visitedIfs;
};


void swapBranches(ASTContext *C, Rewriter &R) {
    BranchSwapper swapper(R);
    MatchFinder Matcher;

    Matcher.addMatcher(ifStmt(unless(hasDescendant(ifStmt()))).bind("ifStmt"), &swapper);
    Matcher.matchAST(*C);
}
