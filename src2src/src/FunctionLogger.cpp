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
      std::stringstream logFunction;
      logFunction << "\n logFunctionyjj(\"" <<  FuncName << "\");\n";
      TheRewriter.InsertText(FuncBody->getLocStart().getLocWithOffset(1), logFunction.str(), true, true);
  }
  return true;
}

