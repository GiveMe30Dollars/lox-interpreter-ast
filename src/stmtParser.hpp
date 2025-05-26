#include "exprParser.hpp"
#include "stmt.hpp"

/*
program        → declaration* EOF ;

declaration    → varDecl
               | statement ;

statement      → exprStmt
               | printStmt ;
*/

class StmtParser : public ExprParser{
    public:
        std::vector<std::shared_ptr<Stmt>> parse();
    protected:
        std::shared_ptr<Stmt> declaration();
        std::shared_ptr<Stmt> varDeclaration();
        std::shared_ptr<Stmt> statement();
        std::shared_ptr<Stmt> exprStatement();
        std::shared_ptr<Stmt> printStatement();
};