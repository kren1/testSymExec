#include "DeadConditionTransformer.hpp"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <random>
#include <sstream>

using namespace clang::ast_matchers;

class FactGenerator {
public:
    virtual std::string getNextTrue() = 0;
    virtual ~FactGenerator(){};
};

class OneIsOneFG : public FactGenerator {
public:
    virtual std::string getNextTrue() {
        return "( 1 == 1 )";
    }
};

class GlobalTuatologyFG : public FactGenerator {
public:
    GlobalTuatologyFG(std::vector<std::tuple<std::string,uint64_t, std::string>> &gv) : global_vars(gv), dis(0, gv.size() -1)  {
        seed.seed(34534530);
    } 
    virtual std::string getNextTrue() {
        if(global_vars.size() == 0) return "( 1 == 1 )";
        //auto rnd_global_var = global_vars.back(); //TODO: implemnt randomnes
        auto rnd_global_var = global_vars[dis(seed)]; //TODO: implemnt randomnes
        std::string name = std::get<0>(rnd_global_var);
        
        uint32_t num = bitRnd(seed);
        std::stringstream fact;
        fact << " ( ";
        fact << name << " <= " << num;
        fact << " | ";
        fact << name << " > " << num; 
        fact << " ) ";
        return fact.str();
    }
private:
    std::vector<std::tuple<std::string,uint64_t, std::string>> &global_vars;
    std::uniform_int_distribution<> dis;
    std::uniform_int_distribution<uint8_t> bitRnd;
    std::mt19937 seed;
};


class ConditionInjector : public MatchFinder::MatchCallback {
public:
  ConditionInjector(Rewriter &Rewrite, FactGenerator &fg) : Rewrite(Rewrite),  factGen(fg), seed(254345), coinFlip(0.5)  {}

  virtual void run(const MatchFinder::MatchResult &Result) {
    if(coinFlip(seed)) return;
    if (const IfStmt *IfS = Result.Nodes.getNodeAs<IfStmt>("ifStmt")) {
      Rewrite.InsertText(IfS->getCond()->getLocStart(),
                         factGen.getNextTrue() + " & ",
                         true,
                         true);
    }
  }

private:
  Rewriter &Rewrite;
  FactGenerator &factGen;
  std::mt19937 seed;
  std::bernoulli_distribution coinFlip;
};

class GlobalVarsRecorder : public MatchFinder::MatchCallback {
public:
  GlobalVarsRecorder(ASTContext *C) : Context(C) {}
  std::vector<std::tuple<std::string,uint64_t, std::string>> global_vars;
  virtual void run(const MatchFinder::MatchResult &Result) {
    if (const VarDecl *decl = Result.Nodes.getNodeAs<VarDecl>("globVar")) {
        std::string var_name = decl->getDeclName().getAsString(); 
        //llvm::errs() << var_name << "================ \n\n ===================== \n";

        APValue *val =  decl->evaluateValue();
        uint64_t size = Context->getTypeInfo(decl->getType()).Width;
        
        global_vars.push_back(std::make_tuple(var_name, size,val->getAsString(*Context, decl->getType())));

    }
  }
private:
 ASTContext *Context;
};

void injectDeadConditions(ASTContext *C, Rewriter &R) {
    GlobalVarsRecorder findGlobalIntegers(C);
    MatchFinder findGV;
    findGV.addMatcher(varDecl(
                              allOf(hasInitializer(expr()), 
                                    hasType(isUnsignedInteger()),
                                    hasGlobalStorage()
                                   )
                             ).bind("globVar"), &findGlobalIntegers );
    findGV.matchAST(*C);
    //llvm::errs() << std::get<0>(findGlobalIntegers.global_vars.front()) << "\n";    

    OneIsOneFG oneIsone;
    GlobalTuatologyFG gt(findGlobalIntegers.global_vars);
    ConditionInjector injector(R, gt);
    MatchFinder Matcher;

    Matcher.addMatcher(ifStmt().bind("ifStmt"), &injector);
    Matcher.matchAST(*C);
}
