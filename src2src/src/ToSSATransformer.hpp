#ifndef _TO_SSA_TRANSFORMER_
#define _TO_SSA_TRANSFORMER_

#include "clang/AST/AST.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include <list>  
#include <string>
#include <map>

using namespace clang;

enum AstState {None, InThen, InElse};

struct ReplaceEntry {
    std::string thenReplace;
    std::string elseReplace;
};

class ToSSATransformer : public RecursiveASTVisitor<ToSSATransformer> {
public:
    ToSSATransformer(Rewriter &R, ASTContext *C);
    bool VisitStmt(Stmt *s);
    bool dataTraverseStmtPre(Stmt *s);
    bool dataTraverseStmtPost(Stmt *s);
    bool shouldTraversePostOrder() const {return true;};
private:
    Rewriter &TheRewriter;
    ASTContext *Context;
    std::list<IfStmt*> ifstack;
    std::string prefix;
    AstState astPosition;
    int uniqueCnt;
    std::map<std::string, ReplaceEntry> varToAlias;
    PrintingPolicy printPolicy;
};
#endif
