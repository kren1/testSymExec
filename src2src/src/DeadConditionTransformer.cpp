#include "DeadConditionTransformer.hpp"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

class FactGenerator {
public:
    virtual std::string getNextTrue() = 0;
    virtual ~FactGenerator(){};
};

class OneIsOneFG : public FactGenerator {
public:
    virtual std::string getNextTrue() {
        return "( 1 == 1 )";
    }
};

class ConditionInjector : public MatchFinder::MatchCallback {
public:
  ConditionInjector(Rewriter &Rewrite, FactGenerator &fg) : Rewrite(Rewrite), factGen(fg) {}

  virtual void run(const MatchFinder::MatchResult &Result) {
    if (const IfStmt *IfS = Result.Nodes.getNodeAs<IfStmt>("ifStmt")) {
      Rewrite.InsertText(IfS->getCond()->getLocStart(),
                         factGen.getNextTrue() + " && ",
                         true,
                         true);
    }
  }

private:
  Rewriter &Rewrite;
  FactGenerator &factGen;
};


void injectDeadConditions(ASTContext *C, Rewriter &R) {
    OneIsOneFG oneIsone;
    ConditionInjector injector(R, oneIsone);
    MatchFinder Matcher;

    Matcher.addMatcher(ifStmt().bind("ifStmt"), &injector);
    Matcher.matchAST(*C);
}
