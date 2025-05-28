// requires statments and the expression parser
#include "exprParser.hpp"
#include "stmt.hpp"

#pragma once

/*
program        → declaration* EOF ;

declaration    → funDecl
               | varDecl
               | statement ;

statement      → exprStmt
               | forStmt
               | ifStmt
               | printStmt
               | whileStmt
               | block ;

block          → "{" declaration* "}" ;
*/

class StmtParser : public ExprParser{
    public:
        StmtParser(std::vector<Token> tokens) : ExprParser(tokens) {}
        std::vector<std::shared_ptr<Stmt>> parse(bool parseExpr = false);
    protected:
        std::shared_ptr<Stmt> declaration(void);
        std::shared_ptr<Stmt> functionDeclaration(std::string kind);
        std::shared_ptr<Stmt> varDeclaration(void);
        std::shared_ptr<Stmt> statement(void);
        std::shared_ptr<Stmt> exprStatement(void);
        std::shared_ptr<Stmt> printStatement(void);
        std::vector<std::shared_ptr<Stmt>> block(void);
        std::shared_ptr<Stmt> ifStatement(void);
        std::shared_ptr<Stmt> whileStatement(void);
        std::shared_ptr<Stmt> forStatement(void);
        std::shared_ptr<Stmt> returnStatement(void);
};