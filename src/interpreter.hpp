// requires expressions, statements and ability to throw LoxErrors
#include "expr.hpp"
#include "stmt.hpp"
#include "loxError.hpp"

// requires Environments (support for variables and scope)
#include "environment.hpp"

// requires LoxCallables (support for functions, classes and methods)
#include "loxCallable.hpp"
#include "loxFunction.hpp"

#pragma once

class Interpreter : public ExprVisitor, public StmtVisitor{
    // Interprets an AST via the Visitor design pattern.
    // Expressions return objects; Statements return void.
    public:
    Interpreter(void);
    Object evaluate(std::shared_ptr<Expr> expr);
    std::any visit(std::shared_ptr<Expr> curr) override;
    void execute(std::shared_ptr<Stmt> stmt);
    void execute(std::vector<std::shared_ptr<Stmt>> statements);
    std::any visit(std::shared_ptr<Stmt> curr) override;

    // EXPR CHILD CLASSES
    std::any visitLiteral(std::shared_ptr<Literal> curr) override;
    std::any visitGrouping(std::shared_ptr<Grouping> curr) override;
    std::any visitUnary(std::shared_ptr<Unary> curr) override;
    std::any visitBinary(std::shared_ptr<Binary> curr) override;
    
    std::any visitVariable(std::shared_ptr<Variable> curr) override;
    std::any visitAssign(std::shared_ptr<Assign> curr) override;
    std::any visitLogical(std::shared_ptr<Logical> curr) override;

    std::any visitCall(std::shared_ptr<Call> curr) override;

    // STMT CHILD CLASSES
    std::any visitExpression(std::shared_ptr<Expression> curr) override;
    std::any visitPrint(std::shared_ptr<Print> curr) override;
    std::any visitVar(std::shared_ptr<Var> curr) override;
    std::any visitBlock(std::shared_ptr<Block> curr) override;
    
    std::any visitIf(std::shared_ptr<If> curr) override;
    std::any visitWhile(std::shared_ptr<While> curr) override;

    std::any visitFunction(std::shared_ptr<Function> curr) override;
    std::any visitReturn(std::shared_ptr<Return> curr) override;

    std::shared_ptr<Environment> globals;
    std::shared_ptr<Environment> env;
    void executeBlock(std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> env);
    LoxError::RuntimeError error(Token op, std::string message);

    private:
    bool isTruthy(Object obj);
    bool isEqual(Object a, Object b);
};