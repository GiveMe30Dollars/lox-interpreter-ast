
#include "expr.hpp"

#pragma once

class ASTPrinter : ExprVisitor{
    // Prints a constructed expression as a string
    // via the Visitor design pattern.
    public:
        std::string print(std::shared_ptr<Expr> expr);
        std::any visit(std::shared_ptr<Expr> curr) override;

        std::any visitLiteral(std::shared_ptr<Literal> curr) override;
        std::any visitGrouping(std::shared_ptr<Grouping> curr) override;
        std::any visitUnary(std::shared_ptr<Unary> curr) override;
        std::any visitBinary(std::shared_ptr<Binary> curr) override;

        std::any visitVariable(std::shared_ptr<Variable> curr) override;
        std::any visitAssign(std::shared_ptr<Assign> curr) override;
};