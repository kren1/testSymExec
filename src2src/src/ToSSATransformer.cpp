#include "ToSSATransformer.hpp"
#include <sstream>
#include <string>
using namespace clang;

ToSSATransformer::ToSSATransformer(Rewriter &R, ASTContext *C) : TheRewriter(R), Context(C), printPolicy(C->getLangOpts()) {}
bool ToSSATransformer::VisitStmt(Stmt *s) {
    if(isa<IfStmt>(s)) {
        IfStmt *ifStmt = cast<IfStmt>(s);
        llvm::errs() << "got and If\n" ;
        Stmt *thenBranch = ifStmt->getThen();
        Stmt *elseBranch = ifStmt->getElse();
        std::string s;
        llvm::raw_string_ostream newIf(s);
        thenBranch->dump();
        thenBranch->printPretty(newIf, nullptr, printPolicy);
        elseBranch->printPretty(newIf, nullptr, printPolicy);
        newIf << "// end of the two;";
        llvm::errs() << newIf.str(); 

        TheRewriter.ReplaceText(SourceRange(ifStmt->getLocStart(), ifStmt->getLocEnd()), s);
    }
    return true;
}
