#include "loxFunction.hpp"
// requires actual Interpreter
#include "interpreter.hpp"
// requires LoxInstance
#include "loxClass.hpp"

int LoxFunction::arity(){
    return declaration->params.size();
}

Object LoxFunction::call(Interpreter& interpreter, std::vector<Object>& arguments){
    std::shared_ptr<Environment> env = std::make_shared<Environment>(closure);
    for (int i = 0; i < declaration->params.size(); i++){
        env->define(declaration->params[i].lexeme, arguments[i]);
    }
    try{
        interpreter.executeBlock(declaration->body, env);
    }
    catch (LoxReturn val){
        return val.obj;
    }
    // return nil by default if no value specified
    return Object::nil();
}

std::string LoxFunction::toString(){
    return "<fn " + declaration->name.lexeme + ">";
}

std::shared_ptr<LoxFunction> LoxFunction::bind(std::shared_ptr<LoxInstance> instance){
    // returns a new function with 'this' keyword binded to instance
    std::shared_ptr<Environment> env = std::make_shared<Environment>(closure);
    env->define("this", Object::instance(instance));
    return std::make_shared<LoxFunction>(declaration, env);
}