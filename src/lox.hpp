#include "token.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

#pragma once

class Lox{
    // Wrapper class to scan, parse and interpret a string
    // as described in the Lox standard
    private:
        static Intepreter interpreter;
    public:
        static void run(std::string source);
        static int hasError;
    

    // Error return classes
    class ScanningError{
        public:
        int line;
        std::string message;
        ScanningError(int line, std::string message) : line(line), message(message) {}
        void print(void) {
            std::cerr << "[line " << line << " ] Error: " << message << "\n";
        }
    };
    class ParsingError{
        public:
        Token token;
        std::string message;
        ParsingError(Token token, std::string message) : token(token), message(message) {}
        void print(void){
            std::cerr << "[line " << token.line << " ] Error at ";
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
            std::cerr << "[line " << line << " ] Error at '" << Token::tokenTypeName.at(token.type) << "': " << message << "\n";
        }
    };
};