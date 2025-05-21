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

const std::unordered_map<TokenType,std::string> tokenTypeName;
const std::unordered_map<std::string, TokenType> reservedKeywords;

bool isDigit(const char c);
bool isAlpha(const char c);
bool isAlphaNumeric(const char c);

class Token {
    public:
        TokenType type;
        std::string lexeme;
        std::string literalString = "null";
        double literalNumber = 0;
        int line;
        Token(TokenType type, std::string lexeme, std::string literalString, int line);
        Token(TokenType type, std::string lexeme, std::string literalString, double literalNumber, int line);
        std::string toString(void);
};