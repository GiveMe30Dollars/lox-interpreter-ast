
// required for std::any for visitor design pattern
#include <any>
// required for smart pointers
#include <memory>
// required for std::move for smart pointers
#include <utility>

// requires tokens
#include "token.hpp"

#pragma once

/*
    HEADER FILES SHOUD DECLARE:
        PUBLIC variables
        PUBLIC functions
        PUBLIC classes
            This includes public and private variables,
            public and private functions,
            and main and delegated constructors.
    Declarations include variable names, functions and their parameters.
*/


//   --EXPRESSION TYPES AND CONSTITUENTS---
/*
expression     → literal
               | unary
               | binary
               | grouping ;

literal        → NUMBER | STRING | "true" | "false" | "nil" ;
grouping       → "(" expression ")" ;
unary          → ( "-" | "!" ) expression ;
binary         → expression operator expression ;
operator       → "==" | "!=" | "<" | "<=" | ">" | ">="
               | "+"  | "-"  | "*" | "/" ;
*/


// To resolve circular compilation dependencies between Expr (and its child classes) and Visitor
// Not including this leads to compile-time errors
class Expr;
class LiteralExpr;
class GroupingExpr;
class UnaryExpr;
class BinaryExpr;
class VariableExpr;
class AssignExpr;
class LogicalExpr;
class CallExpr;
class GetExpr;
class SetExpr;
class ThisExpr;

class ExprVisitor{
    // Abstract class implementing the Visitor design pattern for Expr
    public:
        virtual ~ExprVisitor(void) = default;
        virtual std::any visit(std::shared_ptr<Expr> curr) = 0;

        virtual std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> curr) = 0;
        virtual std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> curr) = 0;
        virtual std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> curr) = 0;
        virtual std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> curr) = 0;

        virtual std::any visitVariableExpr(std::shared_ptr<VariableExpr> curr) = 0;
        virtual std::any visitAssignExpr(std::shared_ptr<AssignExpr> curr) = 0;
        virtual std::any visitLogicalExpr(std::shared_ptr<LogicalExpr> curr) = 0;

        virtual std::any visitCallExpr(std::shared_ptr<CallExpr> curr) = 0;
        virtual std::any visitGetExpr(std::shared_ptr<GetExpr> curr) = 0;
        virtual std::any visitSetExpr(std::shared_ptr<SetExpr> curr) = 0;
        virtual std::any visitThisExpr(std::shared_ptr<ThisExpr> curr) = 0;
};

class Expr{
    // Abstract class implementing expressions.
    // Supports the Visitor design pattern via ExprVisitor, which is passed by reference.
    public:
        virtual ~Expr(void) = default;
        virtual std::any accept(ExprVisitor& v) = 0;
};


// ---CHILD CLASSES (PURE EXPRESSIONS)---
class LiteralExpr : public Expr, public std::enable_shared_from_this<LiteralExpr>{
    // An expression of a literal.
    public:
        Object obj;
        LiteralExpr(Object obj) : obj(obj) {}
        std::any accept(ExprVisitor& v) override { return v.visitLiteralExpr(shared_from_this()); }
};
class GroupingExpr : public Expr, public std::enable_shared_from_this<GroupingExpr>{
    // An expression of a grouping.
    public:
        std::shared_ptr<Expr> expr;
        GroupingExpr(std::shared_ptr<Expr> expr) : expr(expr) {}
        std::any accept(ExprVisitor& v) override { return v.visitGroupingExpr(shared_from_this()); }
};
class UnaryExpr : public Expr, public std::enable_shared_from_this<UnaryExpr>{
    // An expression of a unary operation.
    public:
        Token op;
        std::shared_ptr<Expr> expr;
        UnaryExpr(Token op, std::shared_ptr<Expr> expr) : op(op), expr(expr) {}
        std::any accept(ExprVisitor& v) override { return v.visitUnaryExpr(shared_from_this()); }
};
class BinaryExpr : public Expr, public std::enable_shared_from_this<BinaryExpr>{
    // An expression of a binary operation.
    public:
        std::shared_ptr<Expr> left;
        Token op;
        std::shared_ptr<Expr> right;
        BinaryExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) : left(left), op(op), right(right) {}
        std::any accept(ExprVisitor& v) override { return v.visitBinaryExpr(shared_from_this()); }
};

// ---CHILD CLASSES (VARIABLES)---
class VariableExpr : public Expr, public std::enable_shared_from_this<VariableExpr>{
    // An expression of an l-value (locator value) of a variable.
    public:
        Token name;
        VariableExpr(Token name) :  name(name) {}
        std::any accept(ExprVisitor& v) override { return v.visitVariableExpr(shared_from_this()); }
};
class AssignExpr : public Expr, public std::enable_shared_from_this<AssignExpr>{
    // An expression of an assignment.
    public:
        Token name;
        std::shared_ptr<Expr> expr;
        AssignExpr(Token name, std::shared_ptr<Expr> expr) : name(name), expr(expr) {}
        std::any accept(ExprVisitor& v) override { return v.visitAssignExpr(shared_from_this()); }
};
class LogicalExpr : public Expr, public std::enable_shared_from_this<LogicalExpr>{
    // An expression of a logical operator (AND, OR)
    public:
        std::shared_ptr<Expr> left;
        Token op;
        std::shared_ptr<Expr> right;
        LogicalExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right):
            left(left), op(op), right(right) {}
        std::any accept(ExprVisitor& v) override { return v.visitLogicalExpr(shared_from_this()); }
};

// ---CHILD CLASSES (FUNCTIONS, CLASSES AND METHODS)---
class CallExpr : public Expr, public std::enable_shared_from_this<CallExpr>{
    // An expression for a call by a callable.
    public:
        std::shared_ptr<Expr> callee;
        Token paren;
        std::vector<std::shared_ptr<Expr>> arguments;
        CallExpr(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments) :
            callee(callee), paren(paren), arguments(arguments) {}
        std::any accept(ExprVisitor& v) override { return v.visitCallExpr(shared_from_this()); }
};
class GetExpr : public Expr, public std::enable_shared_from_this<GetExpr>{
    // A get expression: get property [name] from LoxInstance [expr]
    public:
        std::shared_ptr<Expr> expr;
        Token name;
        GetExpr(std::shared_ptr<Expr> expr, Token name) : expr(expr), name(name) {}
        std::any accept(ExprVisitor& v) override { return v.visitGetExpr(shared_from_this()); }
};
class SetExpr : public Expr, public std::enable_shared_from_this<SetExpr>{
    // A set expression: set property [name] from LoxInstance [expr] to [value]
    public:
        std::shared_ptr<Expr> expr;
        Token name;
        std::shared_ptr<Expr> value;
        SetExpr(std::shared_ptr<Expr> expr, Token name, std::shared_ptr<Expr> value) : expr(expr), name(name), value(value) {}
        std::any accept(ExprVisitor& v) override { return v.visitSetExpr(shared_from_this()); }
};
class ThisExpr : public Expr, public std::enable_shared_from_this<ThisExpr>{
    // expression for this keyword
    public:
        Token keyword;
        ThisExpr(Token keyword) : keyword(keyword) {}
        std::any accept(ExprVisitor& v) override { return v.visitThisExpr(shared_from_this()); }
};