#include "token.hpp"

// requires access to system clock for clock()
#include <chrono>

#pragma once

// Interpreter not included: only declaration required
class Interpreter;

class LoxCallable{
    // Abstract class implementing a callable Lox object type
    public:
        virtual ~LoxCallable(void) = default;
        virtual int arity(void) = 0;
        virtual Object call(Interpreter& interpreter, std::vector<Object> arguments) = 0;
        virtual std::string toString(void) = 0;
};

class Clock : public LoxCallable{
    public:
    int arity(void) override { return 0; }
    Object call(Interpreter& interpreter, std::vector<Object> arguments) override{
        using namespace std::chrono;
        return Object::number(
            duration<double>(duration_cast<seconds>( system_clock::now().time_since_epoch() )).count()
        );
    }
    std::string toString(void) override{
        return "<native:clock>";
    }
};