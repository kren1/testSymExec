#include "FunctionLogger.hpp"
#include <sstream>
#include <string>
using namespace clang;

FunctionLogger::FunctionLogger(Rewriter &R, ASTContext *C) : TheRewriter(R), Context(C) {}
bool FunctionLogger::VisitFunctionDecl(FunctionDecl *f) {
  // Only function definitions (with bodies), not declarations.
  if (f->hasBody()) {
      Stmt *FuncBody = f->getBody();
      // Function name
      DeclarationName DeclName = f->getNameInfo().getName();
      std::string FuncName = DeclName.getAsString();
      if(FuncName == "main") return false;
      std::stringstream logFunction;
      logFunction << "\n logFunction(\"" <<  FuncName << "\");\n";
      TheRewriter.InsertText(FuncBody->getLocStart().getLocWithOffset(1), logFunction.str(), true, true);
  }
  return true;
}

int branchId = 0;

bool FunctionLogger::VisitCompoundStmt(CompoundStmt *compStmt) {
      std::stringstream logFunction;
      logFunction << "\n#ifdef BRANCH_LOG\n";
      logFunction << "logFunction(\"" << branchId++ << "\");\n";
      logFunction << "#endif\n";
      TheRewriter.InsertText(compStmt->getLocStart().getLocWithOffset(1), logFunction.str(), true, true);
  return true;
}
