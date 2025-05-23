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
        return Object::number(-isNumber(obj, op));
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
            return Object::boolean(isNumber(left, op) > isNumber(right, op));
        case Token::GREATER_EQUAL:
            return Object::boolean(isNumber(left, op) >= isNumber(right, op));
        case Token::LESS:
            return Object::boolean(isNumber(left, op) < isNumber(right, op));
        case Token::LESS_EQUAL:
            return Object::boolean(isNumber(left, op) <= isNumber(right, op));

        // numeric operators on two numbers
        // string concatenation for '+'
        case Token::PLUS:{
            if (left.type == right.type == Object::NUMBER)
                return Object::number(left.literalNumber + right.literalNumber);
            else if (left.type == right.type == Object::STRING)
                return Object::string(left.literalString + right.literalString);
            else throw error(op, "Expect both number or both string operands.");
        }
        case Token::MINUS:
            return Object::number(isNumber(left, op) - isNumber(right, op));
        case Token::STAR:
            return Object::number(isNumber(left, op) * isNumber(right, op));
        case Token::SLASH:
            return Object::number(isNumber(left, op) / isNumber(right, op));
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
double Interpreter::isNumber(Object obj, Token token){
    // helper function:
    // returns the value if object is a number
    // throws an exception otherwise
    if (obj.type == Object::NUMBER) return obj.literalNumber;
    else throw error(token, "Expect number operand.");
    return 0;
}

LoxError::RuntimeError Interpreter::error(Token token, std::string message){
    return LoxError::RuntimeError(token, message);
}