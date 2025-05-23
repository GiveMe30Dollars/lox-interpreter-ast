#include "token.hpp"

#pragma once

static class LoxError{
    public:
    static void print(int line, std::string message){
        std::cerr << "[line " << line << "] Error: " << message << "\n";
    }
    static void print(Token token, std::string message){
        std::cerr << "[line " << token.line << "] Error at '" << token.lexeme << "': " << message << "\n";
    }
};