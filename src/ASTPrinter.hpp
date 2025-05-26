// requires expressions and statements
#include "expr.hpp"
#include "stmt.hpp"

#pragma once

class ASTPrinter : public ExprVisitor, public StmtVisitor{
    // Prints a constructed expression as a string
    // via the Visitor design pattern.
    // Supports expressions and statements
    public:
        static std::string print(std::shared_ptr<Expr> expr);
        std::any visit(std::shared_ptr<Expr> expr) override;
        static std::string print(std::shared_ptr<Stmt> stmt);
        std::any visit(std::shared_ptr<Stmt> stmt) override;

        // EXPRESSIONS
        std::any visitLiteral(std::shared_ptr<Literal> curr) override;
        std::any visitGrouping(std::shared_ptr<Grouping> curr) override;
        std::any visitUnary(std::shared_ptr<Unary> curr) override;
        std::any visitBinary(std::shared_ptr<Binary> curr) override;

        std::any visitVariable(std::shared_ptr<Variable> curr) override;
        std::any visitAssign(std::shared_ptr<Assign> curr) override;

        // STATEMENTS
        std::any visitExpression(std::shared_ptr<Expression> curr) override;
        std::any visitPrint(std::shared_ptr<Print> curr) override;
        std::any visitVar(std::shared_ptr<Var> curr) override;
        std::any visitBlock(std::shared_ptr<Block> curr) override;
};