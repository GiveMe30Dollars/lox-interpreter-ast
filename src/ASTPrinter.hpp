// requires expressions and statements
#include "expr.hpp"
#include "stmt.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#pragma once

class ASTPrinter : public ExprVisitor, public StmtVisitor{
    // Prints a constructed expression as a string
    // via the Visitor design pattern.
    // Supports expressions and statements
    public:
        std::string print(std::shared_ptr<Expr> expr);
        std::any visit(std::shared_ptr<Expr> expr) override;
        std::string print(std::shared_ptr<Stmt> stmt);
        std::any visit(std::shared_ptr<Stmt> stmt) override;

        // EXPRESSIONS
        std::any visitLiteral(std::shared_ptr<Literal> curr) override;
        std::any visitGrouping(std::shared_ptr<Grouping> curr) override;
        std::any visitUnary(std::shared_ptr<Unary> curr) override;
        std::any visitBinary(std::shared_ptr<Binary> curr) override;

        std::any visitVariable(std::shared_ptr<Variable> curr) override;
        std::any visitAssign(std::shared_ptr<Assign> curr) override;
        std::any visitLogical(std::shared_ptr<Logical> curr) override;

        std::any visitCall(std::shared_ptr<Call> curr) override;
        std::any visitGet(std::shared_ptr<Get> curr) override;
        std::any visitSet(std::shared_ptr<Set> curr) override;
        std::any visitThis(std::shared_ptr<This> curr) override;

        // STATEMENTS
        std::any visitExpression(std::shared_ptr<Expression> curr) override;
        std::any visitPrint(std::shared_ptr<Print> curr) override;
        std::any visitVar(std::shared_ptr<Var> curr) override;
        std::any visitBlock(std::shared_ptr<Block> curr) override;
        
        std::any visitIf(std::shared_ptr<If> curr) override;
        std::any visitWhile(std::shared_ptr<While> curr) override;

        std::any visitFunction(std::shared_ptr<Function> curr) override;
        std::any visitReturn(std::shared_ptr<Return> curr) override;
        std::any visitClass(std::shared_ptr<Class> curr) override;
    private:
        int currIndent = 0;
        int increment = 4;

};