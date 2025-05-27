// requires statments and the expression parser
#include "exprParser.hpp"
#include "stmt.hpp"

#pragma once

/*
program        → declaration* EOF ;

declaration    → varDecl
               | statement ;

statement      → exprStmt
               | ifStmt
               | printStmt
               | block ;

block          → "{" declaration* "}" ;
*/

class StmtParser : public ExprParser{
    public:
        StmtParser(std::vector<Token> tokens) : ExprParser(tokens) {}
        std::vector<std::shared_ptr<Stmt>> parse(bool parseExpr = false);
    protected:
        std::shared_ptr<Stmt> declaration();
        std::shared_ptr<Stmt> varDeclaration();
        std::shared_ptr<Stmt> statement();
        std::shared_ptr<Stmt> exprStatement();
        std::shared_ptr<Stmt> printStatement();
        std::shared_ptr<Stmt> block();
        std::shared_ptr<Stmt> ifStatement();
};