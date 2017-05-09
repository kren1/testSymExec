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
      std::string cond;
      std::string all;
      
      llvm::raw_string_ostream::raw_string_ostream ss(then);
      llvm::raw_string_ostream::raw_string_ostream sse(els);
      llvm::raw_string_ostream::raw_string_ostream ssc(cond);
      llvm::raw_string_ostream::raw_string_ostream ssa(all);

      IfS->getThen()->printPretty(ss,NULL,Policy,0 );
      IfS->getElse()->printPretty(sse,NULL,Policy,0 );
      ssc << "!( ";
      IfS->getCond()->printPretty(ssc,NULL,Policy,0);
      ssc << " -)";

      ssa << "if( !(";
      IfS->getCond()->printPretty(ssa,NULL,Policy,0);
      ssa << " )) ";
      IfS->getElse()->printPretty(ssa,NULL,Policy,0 );
      ssa << "else";
      IfS->getThen()->printPretty(ssa,NULL,Policy,0 );
 
      
      Rewrite.ReplaceText(IfS->getSourceRange(), StringRef(ssa.str()));

      //Rewrite.ReplaceText(IfS->getThen()->getSourceRange(), StringRef(sse.str()));
      //Rewrite.ReplaceText(IfS->getElse()->getSourceRange(), StringRef(ss.str()) );
      //Rewrite.ReplaceText(IfS->getCond()->getSourceRange(), StringRef(ssc.str()) );

      //Rewrite.InsertTextBefore(IfS->getCond()->getLocStart(), "!(");
      //Rewrite.InsertTextAfter(IfS->getCond()->getLocEnd().getLocWithOffset(1), ")");
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
