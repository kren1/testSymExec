#ifndef _TO_SSA_TRANSFORMER_
#define _TO_SSA_TRANSFORMER_

#include "clang/AST/AST.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/RecursiveASTVisitor.h"

using namespace clang;


class ToSSATransformer : public RecursiveASTVisitor<ToSSATransformer> {
public:
    ToSSATransformer(Rewriter &R, ASTContext *C);
    bool VisitStmt(Stmt *s);
private:
    Rewriter &TheRewriter;
    ASTContext *Context;
    PrintingPolicy printPolicy;
};


#endif
