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
std::shared_ptr<LoxFunction> LoxClass::findMethod(std::string s){
    // finds and returns method in class. return nullptr if it doesn't exist.
    if (methods.count(s)) return methods.at(s);
    else return nullptr;
}


std::string LoxInstance::toString(){
    return loxClass->toString() + " instance";
}
Object LoxInstance::get(Token& name){
    // get the property of name [name] 
    // can be field (instance-based) or method (class-based)
    // fields shadow methods
    if (fields.count(name.lexeme)) return fields.at(name.lexeme);

    std::shared_ptr<LoxFunction> func = loxClass->findMethod(name.lexeme);
    if (func) return Object::function(func->bind(shared_from_this()));

    throw LoxError::RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}
void LoxInstance::set(Token& name, Object value){
    // no checking if field exists, as Lox permits addition of fields.
    fields[name.lexeme] = value;
}