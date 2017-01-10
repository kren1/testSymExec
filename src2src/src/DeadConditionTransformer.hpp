#ifndef _DEAD_COND_TRANSFORMER_
#define _DEAD_COND_TRANSFORMER_
#include "clang/AST/AST.h"
#include "clang/Rewrite/Core/Rewriter.h"
using namespace clang;

void injectDeadConditions(ASTContext *C, Rewriter &R);
#endif
