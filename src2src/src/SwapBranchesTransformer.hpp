#ifndef _SWAP_BRANCHES_TRANSFORMER_
#define _SWAP_BRANCHES_TRANSFORMER_
#include "clang/AST/AST.h"
#include "clang/Rewrite/Core/Rewriter.h"
using namespace clang;

void swapBranches(ASTContext *C, Rewriter &R);
#endif
