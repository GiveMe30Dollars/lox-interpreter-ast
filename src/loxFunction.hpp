// innherits forward declaration of Interpreter
#include "loxCallable.hpp"
#include "stmt.hpp"
#include "environment.hpp"

#pragma once

// forward declaration of LoxInstance
class LoxInstance;

class LoxFunction : public LoxCallable{
    // Runtime representation of user-defined Lox function
    // Wrapper for Function : Stmt
    public:
        std::shared_ptr<Function> declaration;
        std::shared_ptr<Environment> closure;
        bool isInitializer;
        LoxFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure, bool isInitializer = false) : 
            declaration(declaration), closure(closure), isInitializer(isInitializer) {}

        int arity(void) override;
        Object call(Interpreter& interpreter, std::vector<Object>& arguments) override;
        std::string toString(void) override;
        
        std::shared_ptr<LoxFunction> bind(std::shared_ptr<LoxInstance> instance);
};