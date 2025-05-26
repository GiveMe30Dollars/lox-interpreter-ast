// requires expressions and its dependencies (tokens)
#include "expr.hpp"


// to resolve circular definitions in StmtVisitor
class Stmt;
class Expression;
class Print;
class Var;
class Block;

class StmtVisitor{
    // Abstract class implementing the Visitor design pattern for Stmt.
    public:
        virtual ~StmtVisitor(void) = default;
        virtual std::any visit(std::shared_ptr<Stmt> curr) = 0;
        
        virtual std::any visitExpression(std::shared_ptr<Expression> curr) = 0;
        virtual std::any visitPrint(std::shared_ptr<Print> curr) = 0;
        virtual std::any visitVar(std::shared_ptr<Var> curr) = 0;
        virtual std::any visitBlock(std::shared_ptr<Block> curr) = 0;
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
        virtual ~Stmt(void) = 0;
        virtual std::any accept(StmtVisitor& v) = 0;
};

// CHILD CLASSES

class Expression : public Stmt, std::enable_shared_from_this<Expression> {
    // A statment wrapping an expression
    // Not to be confused with the abstract class Expr
    public:
        std::shared_ptr<Expr> expr;
        Expression(std::shared_ptr<Expr> expr) : expr(expr) {}
        std::any accept(StmtVisitor& v) override { return v.visitExpression(shared_from_this()); }
};
class Print : public Stmt, std::enable_shared_from_this<Print> {
    // A print statment
    public:
        std::shared_ptr<Expr> expr;
        Print(std::shared_ptr<Expr> expr) : expr(expr) {}
        std::any accept(StmtVisitor& v) override { return v.visitPrint(shared_from_this()); }
};
class Var : public Stmt, std::enable_shared_from_this<Var> {
    // A statement of a variable DECLARATION
    // Not to be confused with Variable : Expr
    public:
        Token name;
        std::shared_ptr<Expr> initializer;
        Var(Token name, std::shared_ptr<Expr> initializer) : name(name), initializer(initializer) {}
        std::any accept(StmtVisitor& v) override { return v.visitVar(shared_from_this()); }
};
class Block : public Stmt, std::enable_shared_from_this<Block> {
    // A statement of a block in lexical scope
    public:
        std::vector<std::shared_ptr<Stmt>> statements;
        Block(std::vector<std::shared_ptr<Stmt>> statements) : statements(statements) {}
        std::any accept(StmtVisitor& v) override { return v.visitBlock(shared_from_this()); }
};