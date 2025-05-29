// requires expressions, statements and ability to throw LoxErrors
#include "expr.hpp"
#include "stmt.hpp"
#include "loxOutput.hpp"

// requires Environments (support for variables and scope)
#include "environment.hpp"

// requires LoxCallables (support for functions, classes and methods)
#include "loxCallable.hpp"
#include "loxFunction.hpp"
#include "loxClass.hpp"

// requires Resolver for resolving and binding
#include "resolver.hpp"

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

        std::shared_ptr<Environment> globals;
        std::shared_ptr<Environment> env;
        void executeBlock(std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> env);
        LoxError::RuntimeError error(Token op, std::string message);

        void resolve(std::shared_ptr<Expr> expr, int steps);
        Object lookUpVariable(Token name, std::shared_ptr<Expr> expr);

    private:
        std::unordered_map<std::shared_ptr<Expr>, int> locals;
        bool isTruthy(Object obj);
        bool isEqual(Object a, Object b);
};