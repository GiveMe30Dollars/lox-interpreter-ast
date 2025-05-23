#include "interpreter.hpp"

Object Interpreter::interpret(std::shared_ptr<Expr> expr){
    return std::any_cast<Object>(visit(expr));
}

std::any Interpreter::visit(std::shared_ptr<Expr> curr){
    return curr->accept(*this);
}

std::any Interpreter::visitLiteral(std::shared_ptr<Literal> curr){
    return curr->obj;
}

std::any Interpreter::visitGrouping(std::shared_ptr<Grouping> curr){
    return interpret(curr->expr);
}

std::any Interpreter::visitUnary(std::shared_ptr<Unary> curr){
    Object obj = interpret(curr->expr);
    if (curr->op.type = Token::BANG){
        return Object::boolean(!obj.isTruthy());
    }
    if (curr->op.type = Token::MINUS){
    }
}

std::any Interpreter::visitBinary(std::shared_ptr<Binary> curr){

}


