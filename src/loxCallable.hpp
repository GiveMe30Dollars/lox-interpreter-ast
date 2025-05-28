#include "token.hpp"
#include "interpreter.hpp"

#pragma once

class LoxCallable{
    // Abstract class implementing a callable Lox object type
    public:
        virtual ~LoxCallable(void) = default;
        virtual int arity(void) = 0;
        virtual Object call(Interpreter& interpreter, std::vector<Object> arguments) = 0;
        virtual std::string toString(void) = 0;
};