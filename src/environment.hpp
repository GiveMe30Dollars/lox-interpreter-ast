// uses Objects
#include "token.hpp"
// required for smart pointers
#include <memory>
// requires LoxErrors
#include "loxError.hpp"

#pragma once

class Environment{
    std::unordered_map<std::string, Object> values = {};
    std::shared_ptr<Environment> enclosing = nullptr;

    public:
    Environment() {}
    Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing) {}
    void define(Token& name, Object value){
        values.insert({name.lexeme, value});
    }
    Object get(Token& name){
        if (values.count(name.lexeme)) return values.at(name.lexeme);
        else if (enclosing) return enclosing->get(name);
        else throw LoxError::RuntimeError(name, "Undefined variable '" + name.lexeme + "'");
    }
};