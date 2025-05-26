// requires expressions, statements and ability to throw LoxErrors
#include "expr.hpp"
#include "stmt.hpp"
#include "loxError.hpp"

// requires Environments (support for variables and scope)
#include "environment.hpp"

#pragma once

class Interpreter : public ExprVisitor, public StmtVisitor{
    // Interprets an AST via the Visitor design pattern.
    // Expressions return objects; Statements return void.
    public:
    Interpreter(void);
    Object evaluate(std::shared_ptr<Expr> expr);
    std::any visit(std::shared_ptr<Expr> curr) override;
    void execute(std::vector<std::shared_ptr<Stmt>> statements);
    std::any visit(std::shared_ptr<Stmt> curr) override;

    // EXPR CHILD CLASSES
    std::any visitLiteral(std::shared_ptr<Literal> curr) override;
    std::any visitGrouping(std::shared_ptr<Grouping> curr) override;
    std::any visitUnary(std::shared_ptr<Unary> curr) override;
    std::any visitBinary(std::shared_ptr<Binary> curr) override;
    
    std::any visitVariable(std::shared_ptr<Variable> curr) override;
    std::any visitAssign(std::shared_ptr<Assign> curr) override;

    // STMT CHILD CLASSES
    std::any visitExpression(std::shared_ptr<Expression> curr) override;
    std::any visitPrint(std::shared_ptr<Print> curr) override;
    std::any visitVar(std::shared_ptr<Var> curr) override;
    std::any visitBlock(std::shared_ptr<Block> curr) override;

    private:
    std::shared_ptr<Environment> env;
    bool isTruthy(Object obj);
    bool isEqual(Object a, Object b);
    LoxError::RuntimeError error(Token op, std::string message);
};