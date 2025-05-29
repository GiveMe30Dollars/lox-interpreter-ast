// requires expressions and its dependencies (tokens)
#include "expr.hpp"

#pragma once


// to resolve circular definitions in StmtVisitor
class StmtStmt;
class ExpressionStmt;
class PrintStmt;
class VarStmt;
class BlockStmt;
class IfStmt;
class WhileStmt;
class FunctionStmt;
class ReturnStmt;
class ClassStmt;

class StmtVisitor{
    // Abstract class implementing the Visitor design pattern for Stmt.
    public:
        virtual ~StmtVisitor(void) = default;
        virtual std::any visit(std::shared_ptr<Stmt> curr) = 0;
        
        virtual std::any visitExpressionStmt(std::shared_ptr<ExpressionStmt> curr) = 0;
        virtual std::any visitPrintStmt(std::shared_ptr<PrintStmt> curr) = 0;
        virtual std::any visitVarStmt(std::shared_ptr<VarStmt> curr) = 0;
        virtual std::any visitBlockStmt(std::shared_ptr<BlockStmt> curr) = 0;

        virtual std::any visitIfStmt(std::shared_ptr<IfStmt> curr) = 0;
        virtual std::any visitWhileStmt(std::shared_ptr<WhileStmt> curr) = 0;

        virtual std::any visitFunctionStmt(std::shared_ptr<FunctionStmt> curr) = 0;
        virtual std::any visitReturnStmt(std::shared_ptr<ReturnStmt> curr) = 0;
        virtual std::any visitClassStmt(std::shared_ptr<ClassStmt> curr) = 0;
};

/*
varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;
printStmt      → "print" expression ";" ;
exprDecl       → expression ";" ;
*/

class Stmt{
    // Abstract class implementing statements.
    // Supports the Visitor design pattern via StmtVisitor, which is passed by reference.
    public:
        virtual ~Stmt(void) = default;
        virtual std::any accept(StmtVisitor& v) = 0;
};


// ---CHILD CLASSES---
// note: enable_shared_from_this MUST BE inherited as PUBLIC

class ExpressionStmt : public Stmt, public std::enable_shared_from_this<ExpressionStmt>{
    // A statment wrapping an expression
    // Not to be confused with the abstract class Expr
    public:
        std::shared_ptr<Expr> expr;
        ExpressionStmt(std::shared_ptr<Expr> expr) : expr(expr) {}
        std::any accept(StmtVisitor& v) override { return v.visitExpressionStmt(shared_from_this()); }
};
class PrintStmt : public Stmt, public std::enable_shared_from_this<PrintStmt>{
    // A print statment
    public:
        std::shared_ptr<Expr> expr;
        PrintStmt(std::shared_ptr<Expr> expr) : expr(expr) {}
        std::any accept(StmtVisitor& v) override { return v.visitPrintStmt(shared_from_this()); }
};
class VarStmt : public Stmt, public std::enable_shared_from_this<VarStmt>{
    // A statement of a variable DECLARATION
    // Not to be confused with Variable : Expr
    public:
        Token name;
        std::shared_ptr<Expr> initializer;
        VarStmt(Token name, std::shared_ptr<Expr> initializer) : name(name), initializer(initializer) {}
        std::any accept(StmtVisitor& v) override { return v.visitVarStmt(shared_from_this()); }
};
class BlockStmt : public Stmt, public std::enable_shared_from_this<BlockStmt>{
    // A statement of a block in lexical scope
    public:
        std::vector<std::shared_ptr<Stmt>> statements;
        BlockStmt(std::vector<std::shared_ptr<Stmt>> statements) : statements(statements) {}
        std::any accept(StmtVisitor& v) override { return v.visitBlockStmt(shared_from_this()); }
};


// ---CHILD CLASSES (CONTROL FLOW)---
class IfStmt : public Stmt, public std::enable_shared_from_this<IfStmt>{
    // A statement encapsulating an if-then-else control flow
    public:
        std::shared_ptr<Expr> condition;
        std::shared_ptr<Stmt> thenBranch;
        std::shared_ptr<Stmt> elseBranch;
        IfStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch) :
            condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}
        std::any accept(StmtVisitor& v) override { return v.visitIfStmt(shared_from_this()); }
};
class WhileStmt : public Stmt, public std::enable_shared_from_this<WhileStmt>{
    // A statement encapsulating a while loop
    public:
        std::shared_ptr<Expr> condition;
        std::shared_ptr<Stmt> body;
        WhileStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body) :
            condition(condition), body(body) {}
        std::any accept(StmtVisitor& v) override { return v.visitWhileStmt(shared_from_this()); }
};


// ---CHILD CLASSES (FUNCTIONS AND CLASSES)---
class FunctionStmt : public Stmt, public std::enable_shared_from_this<FunctionStmt>{
    // A statement encapsulating a function declaration
    public:
        Token name;
        std::vector<Token> params;
        std::vector<std::shared_ptr<Stmt>> body;
        FunctionStmt(Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body) :
            name(name), params(params), body(body) {}
        std::any accept(StmtVisitor& v) override { return v.visitFunctionStmt(shared_from_this()); }
};
class ReturnStmt : public Stmt, public std::enable_shared_from_this<ReturnStmt>{
    // A return statment (from a function or method)
    public:
        Token keyword;
        std::shared_ptr<Expr> expr;
        ReturnStmt(Token keyword, std::shared_ptr<Expr> expr) : keyword(keyword), expr(expr) {}
        std::any accept(StmtVisitor& v) override { return v.visitReturnStmt(shared_from_this()); }
};
class ClassStmt : public Stmt, public std::enable_shared_from_this<ClassStmt>{
    // A statement encapsulating a class declaration
    public:
        Token name;
        std::vector<std::shared_ptr<FunctionStmt>> methods;
        ClassStmt(Token name, std::vector<std::shared_ptr<FunctionStmt>> methods) : name(name), methods(methods) {}
        std::any accept(StmtVisitor& v) override { return v.visitClassStmt(shared_from_this()); }
};