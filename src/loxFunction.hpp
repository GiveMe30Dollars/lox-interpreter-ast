// innherits forward declaration of Interpreter
#include "loxCallable.hpp"
#include "stmt.hpp"
#include "environment.hpp"

#pragma once

class LoxFunction : public LoxCallable{
    // Runtime representation of user-defined Lox function
    // Wrapper for Function : Stmt
    public:
        std::shared_ptr<Function> declaration;
        std::shared_ptr<Environment> closure;
        LoxFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure) : declaration(declaration), closure(closure) {}
        int arity(void) override;
        Object call(Interpreter& interpreter, std::vector<Object>& arguments) override;
        std::string toString(void) override;
};