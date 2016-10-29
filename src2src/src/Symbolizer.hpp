#ifndef _SYMBOLIZER_H_
#define _SYMBOLIZER_H_

#include <sstream>
#include <string>
#include <vector>
#include <tuple>

#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;

class RemoveCrcCallsVisitor : public RecursiveASTVisitor<RemoveCrcCallsVisitor> {
public:
    RemoveCrcCallsVisitor(Rewriter &R, const std::vector<std::tuple<std::string,uint64_t, std::string>> &vars);
    bool VisitStmt(Stmt *s);
private:
    Rewriter &TheRewriter;
    const std::vector<std::tuple<std::string,uint64_t, std::string>> &vars_to_remove;
};

class SymbolizeIntegers : public RecursiveASTVisitor<SymbolizeIntegers> {
public:
  std::vector<std::tuple<std::string,uint64_t, std::string>> global_vars;
  SymbolizeIntegers(Rewriter &R, ASTContext *C);
  bool VisitVarDecl(VarDecl *decl);
  bool VisitFunctionDecl(FunctionDecl *f);
private:
  Rewriter &TheRewriter;
  ASTContext *Context;
};
#endif
