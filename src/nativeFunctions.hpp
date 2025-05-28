// requires LoxCallable, Tokens and reference to Interpreter
#include "loxCallable.hpp"
#include "token.hpp"
#include "interpreter.hpp"

// requires access to system clock
#include <chrono>

#pragma once

namespace Native{
    class Clock : public LoxCallable{
        public:
        int arity(void) override { return 0; }
        Object call(Interpreter& interpreter, std::vector<std::shared_ptr<Object>> arguments) override{
            return std::duration_cast<milliseconds>( system_clock::now().time_since_epoch() );
        }
        std::string toString(void) override{
            return "<native:clock>";
        }
    }
}