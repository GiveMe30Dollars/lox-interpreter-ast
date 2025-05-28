// requires expressions, statements, access to ParseErrors and environments
#include "expr.hpp"
#include "stmt.hpp"
#include "loxOutput.hpp"
#include "environment.hpp"

// implementation uses stacks
#include <deque>

#pragma once

// Interpreter not included: only declaration required
class Interpreter;

class Resolver : public ExprVisitor, public StmtVisitor{
    // Does semantic analysis on ASTs such that
    // scope is statically resolved and variables binded
    /*
        KEY NOTES:
        1. There are no side effects. Input/output of print and native functions suppressed.
        2. There is no control flow.
    */
    public:
        bool hasError = false;

        Resolver(Interpreter& interpreter);
        void resolve(std::shared_ptr<Expr> expr);
        std::any visit(std::shared_ptr<Expr> curr) override;
        void resolve(std::shared_ptr<Stmt> stmt);
        void resolve(std::vector<std::shared_ptr<Stmt>> statements);
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

    private:
        std::deque<std::unordered_map<std::string, bool>> scopes;
        Interpreter& interpreter;
        enum class FunctionType{
            NONE, FUNCTION
        };
        FunctionType currentFunction;

        LoxError::ParseError error(Token token, std::string message);
        void beginScope(void);
        void endScope(void);
        void declare(Token name);
        void define(Token name);
        void resolveLocal(std::shared_ptr<Expr> expr, Token name);
        void resolveFunction(std::shared_ptr<Function> func, FunctionType type);
};