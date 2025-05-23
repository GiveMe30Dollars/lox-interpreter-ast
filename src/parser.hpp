
#include <vector>
// allows variadic functions
#include <cstdarg>

#include "expr.hpp"
#include "error.hpp"

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

class Parser{
    // converts a vector of tokens to an AST
    public:
        bool hasError = false;
        Parser(std::vector<Token> tokens) : tokens(tokens) {}
        std::shared_ptr<Expr> parse();

    private:
        std::vector<Token> tokens;
        int curr = 0;

        bool isAtEnd(void);
        Token advance(void);
        Token peek(void);
        Token previous(void);
        bool check(TokenType t);
        template<typename... args>
        bool match(args... t);
        Token consume(TokenType t, std::string err);

        static class ParseError{};
        ParseError error(Token token, std::string err);

        // Helper parsing functions
        std::shared_ptr<Expr> expression();
        std::shared_ptr<Expr> equality();
        std::shared_ptr<Expr> comparison();
        std::shared_ptr<Expr> term();
        std::shared_ptr<Expr> factor();
        std::shared_ptr<Expr> unary();
        std::shared_ptr<Expr> primary();
};