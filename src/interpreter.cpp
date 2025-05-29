#include "interpreter.hpp"

Interpreter::Interpreter(){
    // initialize global environment, as well as define native functions
    globals = std::make_shared<Environment>(nullptr);
    globals->define("clock", Object::function(std::make_shared<Clock>()));

    env = globals;
    // initialize locals as empty. this will be filled during resolving
    locals = {};
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
std::any Interpreter::visitLiteralExpr(std::shared_ptr<LiteralExpr> curr){
    return curr->obj;
}
std::any Interpreter::visitGroupingExpr(std::shared_ptr<GroupingExpr> curr){
    return evaluate(curr->expr);
}

std::any Interpreter::visitUnaryExpr(std::shared_ptr<UnaryExpr> curr){
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
    else throw error(op, "UNIMPLEMENTED unary operator!");    // Unreachable.
}

std::any Interpreter::visitBinaryExpr(std::shared_ptr<BinaryExpr> curr){
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
            throw error(curr->op, "UNIMPLEMENTED binary operator!");    // Unreachable.
    }
}

std::any Interpreter::visitVariableExpr(std::shared_ptr<VariableExpr> curr){
    // returns stored value as statically resolved by Resolver
    // relies on Resolver being fully implemented
    return lookUpVariable(curr->name, curr);
}
std::any Interpreter::visitAssignExpr(std::shared_ptr<AssignExpr> curr){
    // sets the value of the variable to the evaluated expression,
    // binded to static scope as resolved by Resolver
    // relies on Resolver being fully implemented
    // returns the evaluated expression
    Object obj = evaluate(curr->expr);

    // local variable / global variable
    if (locals.count(curr)) env->assignAt(locals.at(curr), curr->name, obj);
    else globals->assign(curr->name, obj);

    return obj;
}
std::any Interpreter::visitLogicalExpr(std::shared_ptr<LogicalExpr> curr){
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

    // no short circuit. evaluate and return whatever is in curr->right
    return evaluate(curr->right);
}

std::any Interpreter::visitCallExpr(std::shared_ptr<CallExpr> curr){
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
    // (LoxClass is implicitly upcast to LoxCallable)
    std::shared_ptr<LoxCallable> callable;
    if (callee.type == Object::LOX_CALLABLE) callable = callee.loxFunction;
    else callable = callee.loxClass;

    if (arguments.size() != callable->arity())
        throw error(curr->paren, "Expected " + std::to_string(callable->arity()) + " arguments but got " + std::to_string(arguments.size()) + ".");
    
    return callable->call(*this, arguments);
}

std::any Interpreter::visitGetExpr(std::shared_ptr<GetExpr> curr){
    Object obj = evaluate(curr->expr);
    if (obj.type == Object::LOX_INSTANCE){
        return obj.loxInstance->get(curr->name);
    }
    throw error(curr->name, "Only instances have properties.");
}

std::any Interpreter::visitSetExpr(std::shared_ptr<SetExpr> curr){
    Object obj = evaluate(curr->expr);
    if (obj.type == Object::LOX_INSTANCE){
        Object value = evaluate(curr->value);
        obj.loxInstance->set(curr->name, value);
        return value;
    }
    throw error(curr->name, "Only instances have properties.");
}

std::any Interpreter::visitThisExpr(std::shared_ptr<ThisExpr> curr){
    return lookUpVariable(curr->keyword, curr);
}

std::any Interpreter::visitSuperExpr(std::shared_ptr<SuperExpr> curr){
    int distance = locals.at(curr);
    std::shared_ptr<LoxClass> superclass = env->getAt(distance, "super").loxClass;
    std::shared_ptr<LoxInstance> instance = env->getAt(distance - 1, "this").loxInstance;

    std::shared_ptr<LoxFunction> method = superclass->findMethod(curr->method.lexeme);
    return method->bind(instance);
}


/// ---STMT CHILD CLASSES---
std::any Interpreter::visitExpressionStmt(std::shared_ptr<ExpressionStmt> curr){
    // evaluate the expression even if its value is unused
    // this causes eg. 45 + "lorem"; to correctly throw a RuntimeError
    evaluate(curr->expr);
    return nullptr;
}
std::any Interpreter::visitPrintStmt(std::shared_ptr<PrintStmt> curr){
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
std::any Interpreter::visitVarStmt(std::shared_ptr<VarStmt> curr){
    Token name = curr->name;
    Object initializer = curr->initializer ? evaluate(curr->initializer) : Object::nil();
    env->define(name.lexeme, initializer);
    return nullptr;
}
std::any Interpreter::visitBlockStmt(std::shared_ptr<BlockStmt> curr){
    // create new scope for execution of block
    executeBlock(curr->statements, std::make_shared<Environment>(env));
    return nullptr;
}

std::any Interpreter::visitIfStmt(std::shared_ptr<IfStmt> curr){
    if (isTruthy(evaluate(curr->condition))) execute(curr->thenBranch);
    else if (curr->elseBranch) execute(curr->elseBranch);
    return nullptr;
}
std::any Interpreter::visitWhileStmt(std::shared_ptr<WhileStmt> curr){
    while (isTruthy(evaluate(curr->condition)))
        execute(curr->body);
    return nullptr;
}

std::any Interpreter::visitFunctionStmt(std::shared_ptr<FunctionStmt> curr){
    // create and store LoxFunction in local scope
    std::shared_ptr<LoxFunction> func = std::make_shared<LoxFunction>(curr, env);
    env->define(curr->name.lexeme, Object::function(func));
    return nullptr;
}
std::any Interpreter::visitReturnStmt(std::shared_ptr<ReturnStmt> curr){
    // return expression, if any. LoxReturn is caught at end of function call
    Object obj;
    if (curr->expr) obj = evaluate(curr->expr);
    else obj = Object::nil();
    throw LoxReturn(obj);
    return nullptr;    // Unreachable.
}
std::any Interpreter::visitClassStmt(std::shared_ptr<ClassStmt> curr){
    // create and store LoxClass in local scope
    // declares class name first to allow for recursive definitions of functions
    // all methods are also cast from Function:Stmt to LoxFunction

    // get superclass, if any
    Object superclassObj = Object::klass(nullptr);
    if (curr->superclass){
        Object obj = evaluate(curr->superclass);
        if (obj.type != Object::LOX_CLASS)
            throw error(curr->superclass->name, "Superclass must be a class.");
        else superclassObj = obj;
    }

    env->define(curr->name.lexeme, Object::nil());

    // if superclass present, create new nested environment ('super' support)
    if (curr->superclass){
        env = std::make_shared<Environment>(env);
        env->define("super", superclassObj);
    }

    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods = {};
    for (std::shared_ptr<FunctionStmt> method : curr->methods){
        bool isInitializer = method->name.lexeme == "init";
        std::shared_ptr<LoxFunction> loxFunc = std::make_shared<LoxFunction>(method, env, isInitializer);
        methods.insert({method->name.lexeme, loxFunc});
    }

    std::shared_ptr<LoxClass> loxClass = std::make_shared<LoxClass>(curr->name.lexeme, superclassObj.loxClass, methods);

    // end scope for superclass (if any)
    if (curr->superclass) env = env->enclosing;

    env->assign(curr->name, Object::klass(loxClass));
    return nullptr;
}

// ---HELPER FUNCTIONS---

void Interpreter::executeBlock(std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> newScope){
    // change scope to new and execute statements in block. restore scope afterwards
    // if an exception is caught, restore scope before rethrowing
    const std::shared_ptr<Environment> prev = env;
    try{
        env = newScope;
        execute(statements);
        env = prev; 
    }
    catch(...){
        env = prev;
        throw;
    }
}

void Interpreter::resolve(std::shared_ptr<Expr> expr, int steps){
    locals.insert({expr, steps});
}
Object Interpreter::lookUpVariable(Token name, std::shared_ptr<Expr> expr){
    // if locals contains the variable, it is static-scope
    if (locals.count(expr)){
        int distance = locals.at(expr);
        return env->getAt(distance, name);
    }
    // variable is in global scope. fetch and return.
    else return globals->get(name);
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
            throw "UNIMPLEMENTED object type for isEqual!";    // Unreachable.
    }
}

LoxError::RuntimeError Interpreter::error(Token token, std::string message){
    return LoxError::RuntimeError(token, message);
}