#include "DeadConditionTransformer.hpp"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

class BranchSwapper : public MatchFinder::MatchCallback {
public:
  BranchSwapper(Rewriter &Rewrite) : Rewrite(Rewrite) {}

  virtual void run(const MatchFinder::MatchResult &Result) {
    // The matched 'if' statement was bound to 'ifStmt'.
    if (const IfStmt *IfS = Result.Nodes.getNodeAs<IfStmt>("ifStmt")) {
      Rewrite.ReplaceText(IfS->getThen()->getSourceRange(), IfS->getElse()->getSourceRange());
      Rewrite.ReplaceText(IfS->getElse()->getSourceRange(), IfS->getThen()->getSourceRange());

      Rewrite.InsertText(IfS->getCond()->getLocStart(), "!(");
      Rewrite.InsertText(IfS->getCond()->getLocEnd().getLocWithOffset(1), ")");
      
    }
  }

private:
  Rewriter &Rewrite;
};


void swapBranches(ASTContext *C, Rewriter &R) {
    BranchSwapper swapper(R);
    MatchFinder Matcher;

    Matcher.addMatcher(ifStmt(unless(hasDescendant(ifStmt()))).bind("ifStmt"), &swapper);
    Matcher.matchAST(*C);
}
