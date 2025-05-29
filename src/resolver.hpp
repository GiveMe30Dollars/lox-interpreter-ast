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
        3. Errors are not thrown. Once error() is called, the returned ParseError has to be .print()
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
        std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> curr) override;
        std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> curr) override;
        std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> curr) override;
        std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> curr) override;
        
        std::any visitVariableExpr(std::shared_ptr<VariableExpr> curr) override;
        std::any visitAssignExpr(std::shared_ptr<AssignExpr> curr) override;
        std::any visitLogicalExpr(std::shared_ptr<LogicalExpr> curr) override;

        std::any visitCallExpr(std::shared_ptr<CallExpr> curr) override;
        std::any visitGetExpr(std::shared_ptr<GetExpr> curr) override;
        std::any visitSetExpr(std::shared_ptr<SetExpr> curr) override;
        std::any visitThisExpr(std::shared_ptr<ThisExpr> curr) override;
        std::any visitSuperExpr(std::shared_ptr<SuperExpr> curr) override;

        // STMT CHILD CLASSES
        std::any visitExpressionStmt(std::shared_ptr<ExpressionStmt> curr) override;
        std::any visitPrintStmt(std::shared_ptr<PrintStmt> curr) override;
        std::any visitVarStmt(std::shared_ptr<VarStmt> curr) override;
        std::any visitBlockStmt(std::shared_ptr<BlockStmt> curr) override;
        
        std::any visitIfStmt(std::shared_ptr<IfStmt> curr) override;
        std::any visitWhileStmt(std::shared_ptr<WhileStmt> curr) override;

        std::any visitFunctionStmt(std::shared_ptr<FunctionStmt> curr) override;
        std::any visitReturnStmt(std::shared_ptr<ReturnStmt> curr) override;
        std::any visitClassStmt(std::shared_ptr<ClassStmt> curr) override;

    private:
        std::deque<std::unordered_map<std::string, bool>> scopes;
        Interpreter& interpreter;
        enum class FunctionType{
            NONE, FUNCTION, 
            METHOD, INITIALIZER
        };
        enum class ClassType{
            NONE, CLASS, SUBCLASS
        };
        FunctionType currentFunction;
        ClassType currentClass;

        LoxError::ParseError error(Token token, std::string message);
        void beginScope(void);
        void endScope(void);
        void declare(Token name);
        void define(Token name);
        void resolveLocal(std::shared_ptr<Expr> expr, Token name);
        void resolveFunction(std::shared_ptr<FunctionStmt> func, FunctionType type);
};