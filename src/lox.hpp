// requires scanning, parsing and interpreting functionality
#include "scanner.hpp"
#include "stmtParser.hpp"
#include "interpreter.hpp"
// not required, but useful for debugging
//#include "ASTPrinter.hpp"

#pragma once

class Lox{
    // Wrapper class to scan, parse and interpret a string
    // as described in the Lox standard
    private:
        static Interpreter interpreter;
    public:
        static void run(std::string source, bool parseExpr = false);
        static bool hasCompileError;
        static bool hasRuntimeError;
};
