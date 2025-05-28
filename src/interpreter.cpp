#include "interpreter.hpp"

Interpreter::Interpreter(){
    // initialize global environment, as well as define native functions
    globals = std::make_shared<Environment>();
    env = globals;

    globals->define("clock", Object::function(std::make_shared<Clock>()));
}

Object Interpreter::evaluate(std::shared_ptr<Expr> expr){
    return std::any_cast<Object>(visit(expr));
}
std::any Interpreter::visit(std::shared_ptr<Expr> curr){
    return curr->accept(*this);
}

void Interpreter::execute(std::shared_ptr<Stmt> stmt){
    visit(stmt);
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

std::any Interpreter::visitCall(std::shared_ptr<Call> curr){
    // evaluate callee and arguments
    Object callee = evaluate(curr->callee);
    std::vector<Object> arguments = {};
    for (std::shared_ptr<Expr> expr : curr->arguments){
        arguments.push_back(evaluate(expr));
    }

    // if callee is not function or class, throw runtime error
    if (!(callee.type == Object::LOX_CALLABLE || callee.type == Object::LOX_CLASS))
        throw error(curr->paren, "Can only call functions and classes.");
    
    // get LoxCallable from object, check arity and return call value
    std::shared_ptr<LoxCallable> callable;
    if (callee.type == Object::LOX_CALLABLE) callable = callee.loxCallable;
    else throw "UNIMPLEMENTED class Call for Interpreter!";

    if (arguments.size() != callable->arity())
        throw error(curr->paren, "Expected " + std::to_string(callable->arity()) + " arguments but got " + std::to_string(arguments.size()) + ".");
    
    return callable->call(*this, arguments);
}

std::any Interpreter::visitLogical(std::shared_ptr<Logical> curr){
    Object left = evaluate(curr->left);

    // There are only 2 operators: AND, OR
    if (curr->op.type == Token::OR){
        // OR control: short circuit and return "true" (left) if left is truthy
        if (isTruthy(left)) return left;
    }
    else {
        // AND control: short circuit and return "false" (left) if left is falsey
        if (!isTruthy(left)) return left;
    }

    // no short circuit. evaluate and return whatever is inn curr->right
    return evaluate(curr->right);
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

std::any Interpreter::visitIf(std::shared_ptr<If> curr){
    if (isTruthy(evaluate(curr->condition))) execute(curr->thenBranch);
    else if (curr->elseBranch) execute(curr->elseBranch);
    return nullptr;
}
std::any Interpreter::visitWhile(std::shared_ptr<While> curr){
    while (isTruthy(evaluate(curr->condition)))
        execute(curr->body);
    return nullptr;
}

std::any Interpreter::visitFunction(std::shared_ptr<Function> curr){
    // create and store LoxFunction in local scope
    std::shared_ptr<LoxFunction> func = std::make_shared<LoxFunction>(curr);
    env->define(curr->name.lexeme, Object::function(func));
    return nullptr;
}
std::any Interpreter::visitReturn(std::shared_ptr<Return> curr){
    // return expression, if any. LoxReturn is caught at end of function call
    Object obj;
    if (curr->expr) obj = evaluate(curr->expr);
    else obj = Object::nil();
    throw LoxReturn(obj);
    return nullptr;    // Unreachable.
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