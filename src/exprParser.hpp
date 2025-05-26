
#include <vector>
// allows variadic functions
#include <cstdarg>

#include "expr.hpp"
#include "loxError.hpp"

#pragma once

//   ---BASIC PARSING RULES---
/*
expression     → equality ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" ;
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
        std::shared_ptr<Expr> equality();
        std::shared_ptr<Expr> comparison();
        std::shared_ptr<Expr> term();
        std::shared_ptr<Expr> factor();
        std::shared_ptr<Expr> unary();
        std::shared_ptr<Expr> primary();
};