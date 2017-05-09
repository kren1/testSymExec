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

     std::string all;
      
      llvm::raw_string_ostream::raw_string_ostream ssa(all);

      ssa << "if( !(";
      IfS->getCond()->printPretty(ssa,NULL,Policy,0);
      ssa << " )) ";
      IfS->getElse()->printPretty(ssa,NULL,Policy,0 );
      ssa << "else";
      IfS->getThen()->printPretty(ssa,NULL,Policy,0 );
 
      
      Rewrite.ReplaceText(IfS->getSourceRange(), StringRef(ssa.str()));

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
