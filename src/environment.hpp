// uses Objects
#include "token.hpp"
// required for smart pointers
#include <memory>
// requires LoxErrors
#include "loxOutput.hpp"

#pragma once

class Environment : public std::enable_shared_from_this<Environment>{
    std::unordered_map<std::string, Object> values = {};
    public:
    std::shared_ptr<Environment> enclosing = nullptr;

    Environment() {}
    Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing) {}
    void define(std::string name, Object value){
        // defines a variable and its value in this environment
        values[name] = value;
    }

    Object get(Token& name){
        // gets a variable from this and enclosing scopes.
        // value of closest scope returned.
        // throws error if it doesn't exist.
        if (values.count(name.lexeme)) return values.at(name.lexeme);
        else if (enclosing) return enclosing->get(name);
        else throw LoxError::RuntimeError(name, "Undefined variable '" + name.lexeme + "'");
    }
    Object getAt(int distance, std::string s){
        // gets a variable from the ancestor [distance] away from this
        // WARNING: no error handling after resolving
        // note: string used due to also searching for 'this' keyword
        return ancestor(distance)->values.at(s);
    }
    Object getAt(int distance, Token& name){
        return getAt(distance, name.lexeme);
    }

    void assign(Token& name, Object value){
        // assigns the value of a variable in the closest enclosing scopes (incl. this)
        // throws error if it doesn't exist.
        if (values.count(name.lexeme)) values[name.lexeme] = value;
        else if (enclosing) enclosing->assign(name, value);
        else throw LoxError::RuntimeError(name, "Undefined variable '" + name.lexeme + "'");
    }
    void assignAt(int distance, Token& name, Object value){
        // assigns a variable in the ancestor [distance] away from this
        // WARNING: no error handling after resolving
        ancestor(distance)->values.at(name.lexeme) = value;
    }

    private:
    std::shared_ptr<Environment> ancestor(int distance){
        std::shared_ptr<Environment> env = shared_from_this();
        for (int i = 0; i < distance; i++){
            env = env->enclosing;
        }
        return env;
    }
};