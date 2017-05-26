#include "DeadConditionTransformer.hpp"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <set>
#include <string>

using namespace clang::ast_matchers;

class FunctionRemover : public MatchFinder::MatchCallback {
public:
  FunctionRemover(Rewriter &Rewrite) : Rewrite(Rewrite) {}

  virtual void run(const MatchFinder::MatchResult &Result) {
    if (const FunctionDecl *fn = Result.Nodes.getNodeAs<FunctionDecl>("theFunction")) {
//      Rewrite.ReplaceText(IfS->getSourceRange(), StringRef(ssa.str()));
        llvm::errs() << fn->getNameAsString() << "\n\n ============= \n";
        Rewrite.RemoveText(fn->getSourceRange());

    }
  }

private:
  Rewriter &Rewrite;
  std::set<const IfStmt*> visitedIfs;
};


void removeFunction(std::string funcName, ASTContext *C, Rewriter &R) {
    FunctionRemover remover(R);
    MatchFinder Matcher;

    Matcher.addMatcher(functionDecl(hasName(funcName)).bind("theFunction"), &remover);
    Matcher.matchAST(*C);
}
