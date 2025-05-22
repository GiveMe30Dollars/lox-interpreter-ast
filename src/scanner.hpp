
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

class Scanner{
    private:
        std::string source;
        int start;
        int curr;
        int line;
        static std::unordered_map<std::string, TokenType> reservedKeywords;

        bool isAtEnd(void);
        char advance(void);
        char peek(void);
        char peekNext(void);
        bool match(char c);
        void addToken(TokenType type);
        void error(int line, std::string message);

    public:
        bool hasError;
        std::vector<Token> tokens;

        Scanner(std::string source);
        std::vector<Token> scan(void);
    private:
        void scanStringLiteral(void);
        void scanNumber(void);
        void scanIdentifier();
};