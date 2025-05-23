#include "token.hpp"

#pragma once

class LoxError{
    // Error return classes
    public:
    class ScanError{
        public:
        int line;
        std::string message;
        ScanError(int line, std::string message) : line(line), message(message) {}
        void print(void) {
            std::cerr << "[line " << line << "] Error: " << message << "\n";
        }
    };
    class ParseError{
        public:
        Token token;
        std::string message;
        ParseError(Token token, std::string message) : token(token), message(message) {}
        void print(void){
            std::cerr << "[line " << token.line << "] Error at ";
            if (token.type == Token::_EOF) std::cerr << "end: ";
            else std::cerr << "'" << Token::tokenTypeName.at(token.type) << "': ";
            std::cerr << message << "\n";
        }
    };
    class RuntimeError{
        public:
        Token token;
        std::string message;
        RuntimeError(Token token, std::string message) : token(token), message(message) {}
        void print(void){
            std::cerr << "[line " << token.line << "] Error at '" << Token::tokenTypeName.at(token.type) << "': " << message << "\n";
        }
    };
};