#ifndef _TO_SSA_TRANSFORMER_
#define _TO_SSA_TRANSFORMER_

#include "clang/AST/AST.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <list>  
#include <string>

using namespace clang;
using namespace clang::ast_matchers;


class ToSSATransformer : public RecursiveASTVisitor<ToSSATransformer> {
public:
    ToSSATransformer(Rewriter &R, ASTContext *C);
    bool VisitStmt(Stmt *s);
    bool dataTraverseStmtPre(Stmt *s);
    bool dataTraverseStmtPost(Stmt *s);
private:
    Rewriter &TheRewriter;
    ASTContext *Context;
    std::list<IfStmt*> ifstack;
    std::string prefix;
    int uniqueCnt;
    
};

class IfStmtHandler : public MatchFinder::MatchCallback {
public:
  IfStmtHandler(Rewriter &Rewrite, ASTContext *C);
  virtual void run(const MatchFinder::MatchResult &Result);
private:
  Rewriter &Rewrite;
  ASTContext *Context;
  MatchFinder Finder;
};

#endif
