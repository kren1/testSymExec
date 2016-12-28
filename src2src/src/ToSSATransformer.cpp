#include "ToSSATransformer.hpp"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/RefactoringCallbacks.h"
#include <sstream>
using namespace clang;
using namespace clang::ast_matchers;

ToSSATransformer::ToSSATransformer(Rewriter &R, ASTContext *C) : TheRewriter(R), uniqueCnt(0), printPolicy(C->getLangOpts()) {
    llvm::errs() << prefix << "\n";
    llvm::errs() << uniqueCnt << "\n";
    llvm::errs() << ifstack.size() << "\n";
}
bool ToSSATransformer::VisitStmt(Stmt *s) {
    if(isa<IfStmt>(s)) {
        IfStmt *ifStmt = cast<IfStmt>(s);
        llvm::errs() << "got and If\n" ;
        std::ostringstream initVars;
        std::ostringstream ternaryExprs;
        ternaryExprs << "\n";
        for(auto const& mapEntry : varToAlias) {
            std::string type = "int";
            std::string cond;
            const std::string& orig_var = mapEntry.first;
            const std::string& then_var = mapEntry.second.thenReplace;
            const std::string& else_var = mapEntry.second.elseReplace;
            
            llvm::raw_string_ostream cond_stream(cond);
            ifStmt->getCond()->printPretty(cond_stream, nullptr, printPolicy);
            cond_stream.flush();
            uniqueCnt++;
            std::string cond_var = "cond_" + std::to_string(uniqueCnt);

           
            initVars << "char " << cond_var << " = " << cond << ";\n";            
            initVars << type << " " << then_var << " = " << orig_var << ";\n";
            if(!else_var.empty()) 
                initVars << type << " " << else_var << " = " << orig_var << ";\n";

            ternaryExprs << orig_var << " = " << cond_var << " ? " << then_var;
            ternaryExprs << " : " << (else_var.empty() ? orig_var : else_var) << "; \n"; 

        }
        //Remove "else" keyword
        TheRewriter.RemoveText(ifStmt->getElseLoc(), 4);
        //Remove the if with the condition
        TheRewriter.RemoveText(SourceRange(ifStmt-> getLocStart(), ifStmt->getCond()->getLocEnd().getLocWithOffset(2)));
        TheRewriter.InsertTextBefore(ifStmt->getLocStart(), initVars.str());        
        TheRewriter.InsertTextAfter(ifStmt->getLocEnd().getLocWithOffset(1), ternaryExprs.str());        
        varToAlias.clear(); 
      }
    else if(isa<DeclRefExpr>(s) && !ifstack.empty() && astPosition != AstState::None) {
        DeclRefExpr *varRef = cast<DeclRefExpr>(s); 
        std::string varName = varRef->getDecl()->getNameAsString();
        llvm::errs() << "==  " << astPosition << " === \n";

        std::string replaceWith;
        ReplaceEntry& val = varToAlias[varName];
        if(astPosition == AstState::InThen) {
            if(val.thenReplace.empty()) {
                //insert a new one;
                uniqueCnt++;
                val.thenReplace = "t" + std::to_string(uniqueCnt) + varName;
            }
            replaceWith = val.thenReplace;
        } else {
            if(val.elseReplace.empty()) {
                //insert a new one;
                uniqueCnt++;
                val.elseReplace =  "e" + std::to_string(uniqueCnt) + varName;
            }
            replaceWith = val.elseReplace;
        } 
        TheRewriter.ReplaceText(SourceRange(varRef->getLocStart(), varRef->getLocEnd()), replaceWith);
    }
    return true;
}

bool ToSSATransformer::dataTraverseStmtPre(Stmt *s) {
    if(isa<IfStmt>(s)) {
        ifstack.push_back(cast<IfStmt>(s));
    } else if(!ifstack.empty()) {
        if(s == ifstack.back()->getThen()) {
            astPosition = AstState::InThen;
        } else if( s == ifstack.back()->getElse()) {
            astPosition = AstState::InElse;
        } else if( s == ifstack.back()->getCond()) {
            astPosition = AstState::None;

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
    } else if (!ifstack.empty() && (s == ifstack.back()->getThen() || s == ifstack.back()->getElse())) {
        astPosition = AstState::None;
    }
    return true;
}
