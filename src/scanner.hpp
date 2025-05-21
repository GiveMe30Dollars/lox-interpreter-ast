#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cmath>

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
        TokenType type;
        std::string lexeme;
        std::string literalString = "null";
        double literalNumber = 0;
        int line;
        Token(TokenType type, std::string lexeme, std::string literalString);
        Token(TokenType type, std::string lexeme);
        Token(TokenType type, std::string lexeme, std::string literalString, double literalNumber);
        std::string toString(void);

};

class Scanner{
    private:
        std::string source;
        int start;
        int curr;
        int line;
        bool isAtEnd(void);
        char advance(void);
        char peek(void);
        char peekNext(void);
        bool match(char c);
        void addToken(TokenType type);
        void error(int line, std::string message);

    public:
        bool hasError;
        std::vector<Token*> tokens;
        Scanner(std::string source);
    private:
        void scan(void);
        void scanStringLiteral(void);
        void scanNumber(void);
};