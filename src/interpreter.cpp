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
        return Object::boolean(!isTruthy(obj));
    }
    else if (curr->op.type = Token::MINUS){
        if (obj.type == Object::NUMBER){
            obj.literalNumber *= -1;
            return obj;
        }
        else {
            throw error(curr->op, "Operand must be number.");
        }
    }
    else throw error(curr->op, "UNIMPLEMENTED unary operator!");
}

std::any Interpreter::visitBinary(std::shared_ptr<Binary> curr){
    Object objLeft = interpret(curr->left);
    Object objRight = interpret(curr->right);
    switch (curr->op.type){
        case Token::EQUAL_EQUAL:
        case Token::BANG_EQUAL:
        case Token::GREATER:
        case Token::GREATER_EQUAL:
        case Token::LESS:
        case Token::LESS_EQUAL:
        case Token::PLUS:
        case Token::MINUS:
        case Token::STAR:
        case Token::SLASH:
        default:
            throw error(curr->op, "UNIMPLEMENTED unary operator!");
    }
}

bool Interpreter::isTruthy(Object obj){
    return !(obj.type == Object::NIL || (obj.type == Object::BOOL && obj.literalBool == false));
}
bool Interpreter::isEqual(Object a, Object b){
    if (a.type != b.type) return false;
    switch(a.type){
        case Object::NIL:
            return true;
        case Object::BOOL:
            return a.literalBool == b.literalBool;
        case Object::NUMBER:
            return a.literalNumber == b.literalNumber;
        case Object::STRING:
            return a.literalString == b.literalString;
        default:
            throw("UNIMPLEMENTED object type!");
    }
}


