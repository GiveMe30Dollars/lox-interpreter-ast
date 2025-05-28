#include "loxCallable.hpp"
#include "stmt.hpp"

#pragma once

class LoxFunction : public LoxCallable{
    // Runtime representation of user-defined Lox function
    // Wrapper for Function : Stmt
    public:
        std::shared_ptr<Function> declaration;
        LoxFunction(std::shared_ptr<Function> declaration) : declaration(declaration) {}
        int arity(void) override;
        Object call(Interpreter& interpreter, std::vector<Object> arguments) override;
        std::string toString(void) override;
};