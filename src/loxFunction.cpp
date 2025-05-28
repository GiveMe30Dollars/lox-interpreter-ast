#include "loxFunction.hpp"
#include "interpreter.hpp"

int LoxFunction::arity(){
    return declaration->params.size();
}

Object LoxFunction::call(Interpreter& interpreter, std::vector<Object> arguments){
    std::shared_ptr<Environment> env = std::make_shared<Environment>(interpreter.globals);
    for (int i = 0; i < declaration->params.size(); i++){
        env->define(declaration->params[i], arguments[i]);
    }
    interpreter.executeBlock(declaration->body, env);
    return Object::nil();
}

std::string LoxFunction::toString(){
    return "<fn:" + declaration->name.lexeme + ">";
}