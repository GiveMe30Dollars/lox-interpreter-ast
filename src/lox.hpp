#include "token.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

#pragma once

class Lox{
    // Wrapper class to scan, parse and interpret a string
    // as described in the Lox standard
    private:
        static Interpreter interpreter;
    public:
        static void run(std::string source);
        static int hasError;
};
