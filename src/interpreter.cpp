#include "interpreter.hpp"

Interpreter::Interpreter(){
    env = std::make_shared<Environment>();
}

Object Interpreter::evaluate(std::shared_ptr<Expr> expr){
    return std::any_cast<Object>(visit(expr));
}
std::any Interpreter::visit(std::shared_ptr<Expr> curr){
    return curr->accept(*this);
}

void Interpreter::execute(std::vector<std::shared_ptr<Stmt>> statements){
    for (std::shared_ptr<Stmt> stmt : statements) visit(stmt);
    return;
}
std::any Interpreter::visit(std::shared_ptr<Stmt> curr){
    return curr->accept(*this);
}

// ---EXPR CHILD CLASSES---
std::any Interpreter::visitLiteral(std::shared_ptr<Literal> curr){
    return curr->obj;
}
std::any Interpreter::visitGrouping(std::shared_ptr<Grouping> curr){
    return evaluate(curr->expr);
}

std::any Interpreter::visitUnary(std::shared_ptr<Unary> curr){
    Object obj = evaluate(curr->expr);
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
    Object left = evaluate(curr->left);
    Object right = evaluate(curr->right);
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

std::any Interpreter::visitVariable(std::shared_ptr<Variable> curr){
    // returns stored value. if it doesn't exist, throw RuntimeError
    return env->get(curr->name);
}

std::any Interpreter::visitAssign(std::shared_ptr<Assign> curr){
    // sets the value of the variable in the closest enclosing scope
    // (including current scope) to the evaluated expression.
    // returns the evaluated expression
    Object obj = evaluate(curr->expr);
    env->set(curr->name, obj);
    return obj;
}

/// ---STMT CHILD CLASSES---
std::any Interpreter::visitExpression(std::shared_ptr<Expression> curr){
    // evaluate the expression even if its value is unused
    // this causes eg. 45 + "lorem"; to correctly throw a RuntimeError
    evaluate(curr->expr);
    return nullptr;
}
std::any Interpreter::visitPrint(std::shared_ptr<Print> curr){
    Object obj = evaluate(curr->expr);

    // The .0 workaround for Codecrafters.io. You know the deal.
    std::string s;
    if (obj.type == Object::NUMBER){
        double val = obj.literalNumber;
        if (floor(val) == val) s = std::to_string((int)val);
        else s = obj.toString(true);
    }
    else s = obj.toString(true);

    std::cout << s << "\n";
    return nullptr;
}
std::any Interpreter::visitVar(std::shared_ptr<Var> curr){
    Token name = curr->name;
    Object initializer = curr->initializer ? evaluate(curr->initializer) : Object::nil();
    env->define(name, initializer);
    return nullptr;
}
std::any Interpreter::visitBlock(std::shared_ptr<Block> curr){
    // create new scope for execution of block
    executeBlock(curr->statements, std::make_shared<Environment>(env));
    return nullptr;
}


// ---HELPER FUNCTIONS---

void Interpreter::executeBlock(std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> newScope){
    const std::shared_ptr<Environment> prev = env;
    env = newScope;
    execute(statements);
    env = prev; 
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