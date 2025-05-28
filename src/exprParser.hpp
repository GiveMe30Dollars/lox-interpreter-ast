
#include <vector>
// allows variadic functions
#include <cstdarg>

// requires expressions, ability to throw errors
#include "expr.hpp"
#include "loxError.hpp"

#pragma once

//   ---PARSING RULES---
/*
expression     → assignment ;
assignment     → IDENTIFIER "=" assignment
               | logic_or ;
logic_or       → logic_and ( "or" logic_and )* ;
logic_and      → equality ( "and" equality )* ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary | call ;
call           → primary ( "(" arguments? ")" )* ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" | IDENTIFIER ;
*/

class ExprParser{
    // Converts a vector of Tokens to an AST
    public:
        bool hasError = false;
        ExprParser(std::vector<Token> tokens) : tokens(tokens) {}
        std::shared_ptr<Expr> parse();

    protected:
        std::vector<Token> tokens;
        int curr = 0;

        // Helper functions for parsing
        bool isAtEnd(void);
        Token advance(void);
        Token peek(void);
        Token previous(void);
        bool check(Token::TokenType t);
        template<typename... Args>
        bool match(Args... t);
        Token consume(Token::TokenType t, std::string err);

        // Error handlinng and synchronization
        LoxError::ParseError error(Token token, std::string err);
        void synchronize(void);

        // Expression parsing
        std::shared_ptr<Expr> expression();
        std::shared_ptr<Expr> assignment();
        std::shared_ptr<Expr> logicOr();
        std::shared_ptr<Expr> logicAnd();
        std::shared_ptr<Expr> equality();
        std::shared_ptr<Expr> comparison();
        std::shared_ptr<Expr> term();
        std::shared_ptr<Expr> factor();
        std::shared_ptr<Expr> unary();
        std::shared_ptr<Expr> call();
        std::shared_ptr<Expr> primary();

        std::shared_ptr<Expr> finishCall(std::shared_ptr<Expr> callee);
};