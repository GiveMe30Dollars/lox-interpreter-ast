#include "loxFunction.hpp"
// requires actual Interpreter
#include "interpreter.hpp"
// requires LoxInstance
#include "loxClass.hpp"

int LoxFunction::arity(){
    return (int)declaration->params.size();
}

Object LoxFunction::call(Interpreter& interpreter, std::vector<Object>& arguments){
    // create new scope and define all arguments
    std::shared_ptr<Environment> env = std::make_shared<Environment>(closure);
    for (int i = 0; i < declaration->params.size(); i++){
        env->define(declaration->params[i].lexeme, arguments[i]);
    }

    // try execute block. if return value caught, save it
    // if isInitializer, return 'this' (LoxInstance)
    // else return either object thrown or Object::NIL
    // (the Resolver prevents values being returned from initializers)
    Object obj = Object::nil();
    try{
        interpreter.executeBlock(declaration->body, env);
    }
    catch (LoxReturn val){
        obj = val.obj;
    }
    return isInitializer ? closure->getAt(0, "this") : obj;
}

std::string LoxFunction::toString(){
    return "<fn " + declaration->name.lexeme + ">";
}

std::shared_ptr<LoxFunction> LoxFunction::bind(std::shared_ptr<LoxInstance> instance){
    // returns a new function with 'this' keyword binded to instance
    std::shared_ptr<Environment> env = std::make_shared<Environment>(closure);
    env->define("this", Object::instance(instance));
    return std::make_shared<LoxFunction>(declaration, env, isInitializer);
}