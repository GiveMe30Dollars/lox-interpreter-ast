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

const std::unordered_map<TokenType,std::string> tokenTypeName = {
    {LEFT_PAREN, "LEFT_PAREN"}, 
    {RIGHT_PAREN, "RIGHT_PAREN"},
    {LEFT_BRACE, "LEFT_BRACE"}, 
    {RIGHT_BRACE, "RIGHT_BRACE"},

    {COMMA, "COMMA"}, 
    {DOT, "DOT"}, 
    {MINUS, "MINUS"}, 
    {PLUS, "PLUS"},
    {SEMICOLON, "SEMICOLON"}, 
    {SLASH, "SLASH"}, 
    {STAR, "STAR"},

    {BANG, "BANG"}, 
    {BANG_EQUAL, "BANG_EQUAL"}, 
    {EQUAL, "EQUAL"}, 
    {EQUAL_EQUAL, "EQUAL_EQUAL"},
    {GREATER, "GREATER"}, 
    {GREATER_EQUAL, "GREATER_EQUAL"},
    {LESS, "LESS"}, 
    {LESS_EQUAL, "LESS_EQUAL"},

    {IDENTIFIER, "IDENTIFIER"}, 
    {STRING, "STRING"}, 
    {NUMBER, "NUMBER"},

    {AND,"AND"}, 
    {CLASS,"CLASS"}, 
    {ELSE,"ELSE"}, 
    {FALSE,"FALSE"},
    {FUN,"FUN"}, 
    {FOR,"FOR"}, 
    {IF,"IF"}, 
    {NIL,"NIL"}, 
    {OR,"OR"},
    {PRINT,"PRINT"}, 
    {RETURN,"RETURN"}, 
    {SUPER,"SUPER"}, 
    {THIS,"THIS"},
    {TRUE,"TRUE"}, 
    {VAR,"VAR"}, 
    {WHILE,"WHILE"},
    
    {_EOF, "EOF"}
};

const std::unordered_map<std::string, TokenType> reservedKeywords = {
    {"and", AND},
    {"class", CLASS},
    {"else", ELSE},
    {"false", FALSE},
    {"fun", FUN},
    {"for", FOR},
    {"if", IF},
    {"nil", NIL},
    {"or", OR},
    {"print", PRINT},
    {"return", RETURN},
    {"super", SUPER},
    {"this", THIS},
    {"true", TRUE},
    {"var", VAR},
    {"while", WHILE},
};

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