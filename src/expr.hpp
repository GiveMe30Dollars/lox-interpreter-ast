
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
class Variable;
class Assign;
class Logical;
class Call;
class Get;
class Set;
class This;

class ExprVisitor{
    // Abstract class implementing the Visitor design pattern for Expr
    public:
        virtual ~ExprVisitor(void) = default;
        virtual std::any visit(std::shared_ptr<Expr> curr) = 0;

        virtual std::any visitLiteral(std::shared_ptr<Literal> curr) = 0;
        virtual std::any visitGrouping(std::shared_ptr<Grouping> curr) = 0;
        virtual std::any visitUnary(std::shared_ptr<Unary> curr) = 0;
        virtual std::any visitBinary(std::shared_ptr<Binary> curr) = 0;

        virtual std::any visitVariable(std::shared_ptr<Variable> curr) = 0;
        virtual std::any visitAssign(std::shared_ptr<Assign> curr) = 0;
        virtual std::any visitLogical(std::shared_ptr<Logical> curr) = 0;

        virtual std::any visitCall(std::shared_ptr<Call> curr) = 0;
        virtual std::any visitGet(std::shared_ptr<Get> curr) = 0;
        virtual std::any visitSet(std::shared_ptr<Set> curr) = 0;
        virtual std::any visitThis(std::shared_ptr<This> curr) = 0;
};

class Expr{
    // Abstract class implementing expressions.
    // Supports the Visitor design pattern via ExprVisitor, which is passed by reference.
    public:
        virtual ~Expr(void) = default;
        virtual std::any accept(ExprVisitor& v) = 0;
};


// ---CHILD CLASSES (PURE EXPRESSIONS)---
class Literal : public Expr, public std::enable_shared_from_this<Literal>{
    // An expression of a literal.
    public:
        Object obj;
        Literal(Object obj) : obj(obj) {}
        std::any accept(ExprVisitor& v) override { return v.visitLiteral(shared_from_this()); }
};
class Grouping : public Expr, public std::enable_shared_from_this<Grouping>{
    // An expression of a grouping.
    public:
        std::shared_ptr<Expr> expr;
        Grouping(std::shared_ptr<Expr> expr) : expr(expr) {}
        std::any accept(ExprVisitor& v) override { return v.visitGrouping(shared_from_this()); }
};
class Unary : public Expr, public std::enable_shared_from_this<Unary>{
    // An expression of a unary operation.
    public:
        Token op;
        std::shared_ptr<Expr> expr;
        Unary(Token op, std::shared_ptr<Expr> expr) : op(op), expr(expr) {}
        std::any accept(ExprVisitor& v) override { return v.visitUnary(shared_from_this()); }
};
class Binary : public Expr, public std::enable_shared_from_this<Binary>{
    // An expression of a binary operation.
    public:
        std::shared_ptr<Expr> left;
        Token op;
        std::shared_ptr<Expr> right;
        Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) : left(left), op(op), right(right) {}
        std::any accept(ExprVisitor& v) override { return v.visitBinary(shared_from_this()); }
};

// ---CHILD CLASSES (VARIABLES)---
class Variable : public Expr, public std::enable_shared_from_this<Variable>{
    // An expression of an l-value (locator value) of a variable.
    public:
        Token name;
        Variable(Token name) :  name(name) {}
        std::any accept(ExprVisitor& v) override { return v.visitVariable(shared_from_this()); }
};
class Assign : public Expr, public std::enable_shared_from_this<Assign>{
    // An expression of an assignment.
    public:
        Token name;
        std::shared_ptr<Expr> expr;
        Assign(Token name, std::shared_ptr<Expr> expr) : name(name), expr(expr) {}
        std::any accept(ExprVisitor& v) override { return v.visitAssign(shared_from_this()); }
};
class Logical : public Expr, public std::enable_shared_from_this<Logical>{
    // An expression of a logical operator (AND, OR)
    public:
        std::shared_ptr<Expr> left;
        Token op;
        std::shared_ptr<Expr> right;
        Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right):
            left(left), op(op), right(right) {}
        std::any accept(ExprVisitor& v) override { return v.visitLogical(shared_from_this()); }
};

// ---CHILD CLASSES (FUNCTIONS, CLASSES AND METHODS)---
class Call : public Expr, public std::enable_shared_from_this<Call>{
    // An expression for a call by a callable.
    public:
        std::shared_ptr<Expr> callee;
        Token paren;
        std::vector<std::shared_ptr<Expr>> arguments;
        Call(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments) :
            callee(callee), paren(paren), arguments(arguments) {}
        std::any accept(ExprVisitor& v) override { return v.visitCall(shared_from_this()); }
};
class Get : public Expr, public std::enable_shared_from_this<Get>{
    // A get expression: get property [name] from LoxInstance [expr]
    public:
        std::shared_ptr<Expr> expr;
        Token name;
        Get(std::shared_ptr<Expr> expr, Token name) : expr(expr), name(name) {}
        std::any accept(ExprVisitor& v) override { return v.visitGet(shared_from_this()); }
};
class Set : public Expr, public std::enable_shared_from_this<Set>{
    // A set expression: set property [name] from LoxInstance [expr] to [value]
    public:
        std::shared_ptr<Expr> expr;
        Token name;
        std::shared_ptr<Expr> value;
        Set(std::shared_ptr<Expr> expr, Token name, std::shared_ptr<Expr> value) : expr(expr), name(name), value(value) {}
        std::any accept(ExprVisitor& v) override { return v.visitSet(shared_from_this()); }
};
class This : public Expr, public std::enable_shared_from_this<This>{
    // expression for this keyword
    public:
        Token keyword;
        This(Token keyword) :keyword(keyword) {}
        std::any accept(ExprVisitor& v) override { return v.visitThis(shared_from_this()); }
};