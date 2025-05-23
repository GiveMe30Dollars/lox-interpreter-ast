
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


// To resolve circular compilation dependencies between Expr, 
// its child classes, and Visitor
// Not including this leads to compile-time errors
class Expr;
class Literal;
class Grouping;
class Unary;
class Binary;

class Visitor{
    // Abstract class implementing the Visitor design pattern for Expr. supports any return type
    public:
        virtual ~Visitor(void) = default;
        virtual std::any visit(std::shared_ptr<Expr> expr) = 0;
        virtual std::any visitLiteral(std::shared_ptr<Literal> expr) = 0;
        virtual std::any visitGrouping(std::shared_ptr<Grouping> expr) = 0;
        virtual std::any visitUnary(std::shared_ptr<Unary> expr) = 0;
        virtual std::any visitBinary(std::shared_ptr<Binary> expr) = 0;
};

class Expr{
    // Abstract class implementing expressions. Supports the Visitor pattern with variable return type.
    // Visitor is passed by reference.
    public:
        virtual ~Expr(void) = default;
        virtual std::any accept(Visitor& v) = 0;
};



class Literal : public Expr, public std::enable_shared_from_this<Literal>{
    // An expression of a literal.
    public:
        Object obj;
        Literal(Object obj) : obj(obj) {}
        std::any accept(Visitor& v) override { return v.visitLiteral(shared_from_this()); }
};

class Grouping : public Expr, public std::enable_shared_from_this<Grouping>{
    // An expression of a grouping.
    public:
        std::shared_ptr<Expr> expr;
        Grouping(std::shared_ptr<Expr> expr) : expr(expr) {}
        std::any accept(Visitor& v) override { return v.visitGrouping(shared_from_this()); }
};

class Unary : public Expr, public std::enable_shared_from_this<Unary>{
    // An expression of a unary operation.
    public:
        Token op;
        std::shared_ptr<Expr> expr;
        Unary(Token op, std::shared_ptr<Expr> expr) : op(op), expr(expr) {}
        std::any accept(Visitor& v) override { return v.visitUnary(shared_from_this()); }
};

class Binary : public Expr, public std::enable_shared_from_this<Binary>{
    // An expression of a binary operation.
    public:
        std::shared_ptr<Expr> left;
        Token op;
        std::shared_ptr<Expr> right;
        Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) : left(left), op(op), right(right) {}
        std::any accept(Visitor& v) override { return v.visitBinary(shared_from_this()); }
};

