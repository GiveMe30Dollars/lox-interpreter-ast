// inherits forward declaration of Interpreter
#include "loxCallable.hpp"
#include "loxFunction.hpp"

#pragma once

class LoxClass : public LoxCallable, public std::enable_shared_from_this<LoxClass>{
    public:
        std::string name;
        std::shared_ptr<LoxClass> superclass;
        std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;
        LoxClass(std::string name, std::shared_ptr<LoxClass> superclass, 
            std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods) :
            name(name), superclass(superclass), methods(methods) {}

        int arity(void) override;
        Object call(Interpreter& interpreter, std::vector<Object>& arguments) override;
        std::string toString(void);
        std::shared_ptr<LoxFunction> findMethod(std::string s);
};

class LoxInstance : public std::enable_shared_from_this<LoxInstance>{
    public:
        std::shared_ptr<LoxClass> loxClass;
        LoxInstance(std::shared_ptr<LoxClass> loxClass) : loxClass(loxClass) {}
        std::string toString(void);
        Object get(Token& name);
        void set(Token& name, Object value);
    private:
        std::unordered_map<std::string, Object> fields = {};
};