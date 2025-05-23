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
    Token op = curr->op;
    if (op.type == Token::BANG){
        return Object::boolean(!isTruthy(obj));
    }
    else if (op.type == Token::MINUS){
        if (obj.type == Object::NUMBER) 
            return Object::number(-obj.literalNumber);
        else throw error(op, "Operand must be a number.");
    }
    else throw error(op, "UNIMPLEMENTED unary operator!");
}

std::any Interpreter::visitBinary(std::shared_ptr<Binary> curr){
    Object left = interpret(curr->left);
    Object right = interpret(curr->right);
    Token op = curr->op;

    switch (op.type){
        // boolean operators based on truthiness
        case Token::EQUAL_EQUAL:
            return Object::boolean(isEqual(left, right));
        case Token::BANG_EQUAL:
            return Object::boolean(!isEqual(left, right));

        // boolean operators on two numbers
        case Token::GREATER:
            if (left.type == Object::NUMBER && left.type == right.type)
                return Object::boolean(left.literalNumber > right.literalNumber);
            else throw error(op, "Operands must be numbers.");
        case Token::GREATER_EQUAL:
            if (left.type == Object::NUMBER && left.type == right.type)
                return Object::boolean(left.literalNumber >= right.literalNumber);
            else throw error(op, "Operands must be numbers.");
        case Token::LESS:
            if (left.type == Object::NUMBER && left.type == right.type)
                return Object::boolean(left.literalNumber < right.literalNumber);
            else throw error(op, "Operands must be numbers.");
        case Token::LESS_EQUAL:
            if (left.type == Object::NUMBER && left.type == right.type)
                return Object::boolean(left.literalNumber <= right.literalNumber);
            else throw error(op, "Operands must be numbers.");

        // numeric operators on two numbers
        // additionally, string concatenation for '+'
        case Token::PLUS:{
            if (left.type == Object::NUMBER && left.type == right.type)
                return Object::number(left.literalNumber + right.literalNumber);
            else if (left.type== Object::STRING && left.type == right.type)
                return Object::string(left.literalString + right.literalString);
            else throw error(op, "Operands must be numbers or strings.");
        }
        case Token::MINUS:
            if (left.type == Object::NUMBER && left.type == right.type)
                return Object::number(left.literalNumber - right.literalNumber);
            else throw error(op, "Operands must be numbers.");
        case Token::STAR:
            if (left.type == Object::NUMBER && left.type == right.type)
                return Object::number(left.literalNumber * right.literalNumber);
            else throw error(op, "Operands must be numbers.");
        case Token::SLASH:
            // TODO: NAN implementation for division by 0
            if (left.type == Object::NUMBER && left.type == right.type)
                return Object::number(left.literalNumber / right.literalNumber);
            else throw error(op, "Operands must be numbers.");

        default:
            throw error(curr->op, "UNIMPLEMENTED binary operator!");
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
            throw "UNIMPLEMENTED object type for isEqual!";
    }
}

LoxError::RuntimeError Interpreter::error(Token token, std::string message){
    return LoxError::RuntimeError(token, message);
}