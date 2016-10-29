#ifndef _FUNCTION_LOGGER_
#define _FUNCTION_LOGGER_

#include "clang/AST/AST.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/RecursiveASTVisitor.h"

using namespace clang;


class FunctionLogger : public RecursiveASTVisitor<FunctionLogger> {
public:
    FunctionLogger(Rewriter &R, ASTContext *C);
    bool VisitFunctionDecl(FunctionDecl *f);
private:
    Rewriter &TheRewriter;
    ASTContext *Context;
};


#endif
