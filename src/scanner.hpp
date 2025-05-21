#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <unordered_set>
#include <unordered_map>

#pragma once

enum TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    _EOF
};

std::string tokenTypeToString(const TokenType type);
bool isDigit(const char c);
bool isLetter(const char c);

class Token {
    public:
        Token(TokenType type, std::string lexeme, std::string literal);
        Token(TokenType type, std::string lexeme);
        std::string toString(void);
};

class Scanner{
    private:
        bool isAtEnd(void);
        char advance(void);
        char peek(void);
        char peekNext(void);
        bool match(char c);
        void addToken(TokenType type);

    public:
        bool hasError;
        std::vector<Token*> tokens;
        Scanner(std::string source);
    private:
        void scan(void);
};