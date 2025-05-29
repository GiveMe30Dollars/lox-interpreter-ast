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

        // STATEMENTS
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
        int currIndent = 0;
        int increment = 2;

};