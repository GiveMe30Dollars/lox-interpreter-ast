#include "loxClass.hpp"
// requires actual Interpreter
#include "interpreter.hpp"

int LoxClass::arity(){
    return 0;
}
std::string LoxClass::toString(){
    return name;
}
Object LoxClass::call(Interpreter& interpreter, std::vector<Object>& arguments){
    std::shared_ptr<LoxInstance> instance = std::make_shared<LoxInstance>(shared_from_this());
    return Object::instance(instance);
}

std::string LoxInstance::toString(){
    return loxClass->toString() + " instance";
}