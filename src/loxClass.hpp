// inherits forward declaration of Interpreter
#include "loxCallable.hpp"

#pragma once

class LoxClass : public LoxCallable, public std::enable_shared_from_this<LoxClass>{
    public:
        std::string name;
        LoxClass(std::string name) : name(name) {}
        int arity(void) override;
        Object call(Interpreter& interpreter, std::vector<Object>& arguments) override;
        std::string toString(void);
};

class LoxInstance : public std::enable_shared_from_this<LoxInstance>{
    public:
        std::shared_ptr<LoxClass> loxClass;
        LoxInstance(std::shared_ptr<LoxClass> loxClass) : loxClass(loxClass) {}
        std::string toString(void);
};