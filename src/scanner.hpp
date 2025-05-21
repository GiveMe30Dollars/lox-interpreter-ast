#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <unordered_set>
#include <unordered_map>

#pragma once

enum TokenType;
std::string tokenTypeToString(const TokenType type);
bool isDigit(const char c);
bool isLetter(const char c);

class Token {
    private:
        TokenType type;
        std::string lexeme;
        std::string literal;
        int line;
    public:
        Token(TokenType type, std::string lexeme, std::string literal) {}
        Token(TokenType type, std::string lexeme) : Token(type, lexeme, "null") {}
        std::string toString(void) {}
};

class Scanner{
    private:
        std::string source;
        int start;
        int curr;
        int line;
        const std::unordered_set<char> symbols;
        bool isAtEnd(void){}
        char advance(void){}
        char peek(void){}
        char peekNext(void){}
        bool match(char c){}
        void addToken(TokenType type){}

    public:
        bool hasError;
        std::vector<Token*> tokens;
        Scanner(std::string source){}
    private:
        void scan(void){}
};