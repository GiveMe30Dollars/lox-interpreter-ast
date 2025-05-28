#include "loxFunction.hpp"
#include "interpreter.hpp"

int LoxFunction::arity(){
    return declaration->params.size();
}

Object LoxFunction::call(Interpreter& interpreter, std::vector<Object> arguments){
    std::shared_ptr<Environment> env = std::make_shared<Environment>(closure);
    for (int i = 0; i < declaration->params.size(); i++){
        env->define(declaration->params[i], arguments[i]);
    }
    try{
        std::cerr << "running n = " << arguments[0].toString() << "\n";
        interpreter.executeBlock(declaration->body, env);
    }
    catch (LoxReturn val){
        std::cerr << "fibonacci number: " << arguments[0].toString() << " -> " << val.obj.toString() << "\n";
        return val.obj;
    }
    // return nil by default if no value specified
    return Object::nil();
}

std::string LoxFunction::toString(){
    return "<fn " + declaration->name.lexeme + ">";
}