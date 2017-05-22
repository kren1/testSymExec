#ifndef _REMOVE_FUNCTION_
#define _REMOVE_FUNCTION_
#include "clang/AST/AST.h"
#include "clang/Rewrite/Core/Rewriter.h"
using namespace clang;

void removeFunction(std::string funcName,ASTContext *C, Rewriter &R);
#endif
