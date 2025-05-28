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
    void define(std::string name, Object value){
        // defines a variable and its value.
        values[name] = value;
    }
    void define(Token& name, Object value){
        define(name.lexeme, value);
    }
    Object get(Token& name){
        // gets a variable from this and enclosing scopes.
        // value of closest scope returned.
        // throws error if it doesn't exist.
        if (values.count(name.lexeme)) return values.at(name.lexeme);
        else if (enclosing) return enclosing->get(name);
        else throw LoxError::RuntimeError(name, "Undefined variable '" + name.lexeme + "'");
    }
    void set(Token& name, Object value){
        // sets the value of a variable in the closest enclosing scopes (incl. this)
        // throws error if it doesn't exist.
        if (values.count(name.lexeme)) values[name.lexeme] = value;
        else if (enclosing) enclosing->set(name, value);
        else throw LoxError::RuntimeError(name, "Undefined variable '" + name.lexeme + "'");
    }
};