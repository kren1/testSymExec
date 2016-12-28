#include "ToSSATransformer.hpp"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/RefactoringCallbacks.h"
#include <sstream>
using namespace clang;
using namespace clang::ast_matchers;

ToSSATransformer::ToSSATransformer(Rewriter &R, ASTContext *C) : TheRewriter(R), Context(C), uniqueCnt(0) {
    llvm::errs() << prefix << "\n";
    llvm::errs() << uniqueCnt << "\n";
    llvm::errs() << ifstack.size() << "\n";
}
bool ToSSATransformer::VisitStmt(Stmt *s) {
    if(isa<IfStmt>(s)) {
        IfStmt *ifStmt = cast<IfStmt>(s);
        llvm::errs() << "got and If\n" ;
    /*    std::string s1;
        llvm::raw_string_ostream newIf(s1);
        thenBranch->dump();
        thenBranch->printPretty(newIf, nullptr, printPolicy);
        elseBranch->printPretty(newIf, nullptr, printPolicy);
        newIf << "// end of the two;";
        llvm::errs() << newIf.str(); */

//        TheRewriter.ReplaceText(SourceRange(ifStmt->getLocStart(), ifStmt->getLocEnd()), s);
//        auto r = clang::tooling::replaceStmtWithStmt(Context->getSourceManager(), *s, *thenBranch);
    }
    else if(isa<DeclRefExpr>(s) && !ifstack.empty()) {
        DeclRefExpr *varRef = cast<DeclRefExpr>(s); 
        llvm::errs() << "   -  " << ifstack.size()  << "\n";
        llvm::errs() << "\n ==  " << prefix.size() << " === \n";
        TheRewriter.InsertTextBefore(varRef->getLocStart(), prefix);

    }
    return true;
}

bool ToSSATransformer::dataTraverseStmtPre(Stmt *s) {
    if(isa<IfStmt>(s)) {
        ifstack.push_back(cast<IfStmt>(s));
    } else if(!ifstack.empty()) {
        std::ostringstream stringStream;
        if(s == ifstack.back()->getThen()) {
            llvm::errs() << "THEN!!!";
            stringStream << "then_" << uniqueCnt << "_";            
            prefix = stringStream.str();
            uniqueCnt++;
        } else if( s == ifstack.back()->getElse()) {
            llvm::errs() << "else!!!";
            stringStream << "else_" << uniqueCnt << "_";            
            prefix = stringStream.str();
            uniqueCnt++;
        }
    }
    return true;
}

bool ToSSATransformer::dataTraverseStmtPost(Stmt *s) {
    if(isa<IfStmt>(s)) {
        llvm::errs() << "popping:  " << s << "\n";
        if( s != ifstack.back())
            llvm::errs() << "Popped different if, investigate !!! \n";
        ifstack.pop_back();
    }
    return true;
}

IfStmtHandler::IfStmtHandler(Rewriter &Rewrite, ASTContext *C) : Rewrite(Rewrite), Context(C) {}

void IfStmtHandler::run(const MatchFinder::MatchResult &Result) {
    llvm::errs() << "matched! ihihi \n";
    // The matched 'if' statement was bound to 'ifStmt'.
    if (const IfStmt *IfS = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt")) {
        llvm::errs() << "matched ifi \n";
      const Stmt *Then = IfS->getThen();
      Rewrite.InsertText(Then->getLocStart(), "// the 'if' part\n", true, true);

      if (const Stmt *Else = IfS->getElse()) {
        Rewrite.InsertText(Else->getLocStart(), "// the 'else' part\n", true,
                           true);
      }
    }
    else if(const DeclRefExpr *varRef = Result.Nodes.getNodeAs<DeclRefExpr>("refExpr")) {
        llvm::errs() << "matched decl\n";
        if(const ValueDecl* vd = varRef->getDecl()) {
            llvm::errs() << vd->getNameAsString() << "\n";
        }

    } 

}
