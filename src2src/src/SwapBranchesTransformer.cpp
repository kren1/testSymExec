#include "DeadConditionTransformer.hpp"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

class ConditionInjector : public MatchFinder::MatchCallback {
public:
  ConditionInjector(Rewriter &Rewrite) : Rewrite(Rewrite) {}

  virtual void run(const MatchFinder::MatchResult &Result) {
    // The matched 'if' statement was bound to 'ifStmt'.
    if (const IfStmt *IfS = Result.Nodes.getNodeAs<IfStmt>("ifStmt")) {
      Rewrite.InsertText(IfS->getCond()->getLocStart(),
                         "true && ",
                         true,
                         true);
    }
  }

private:
  Rewriter &Rewrite;
};


void swapBranches(ASTContext *C, Rewriter &R) {
    ConditionInjector injector(R);
    MatchFinder Matcher;

    Matcher.addMatcher(ifStmt().bind("ifStmt"), &injector);
    Matcher.matchAST(*C);
}
