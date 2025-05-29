#include "resolver.hpp"

// requires implemetation details of Interpreter
// (previously declared separately)
#include "interpreter.hpp"


Resolver::Resolver(Interpreter& interpreter) : interpreter(interpreter){
    // interpreter has to be passed as member initializer
    hasError = false;
    scopes = {};
    currentFunction = FunctionType::NONE;
}
void Resolver::resolve(std::shared_ptr<Expr> expr){
    visit(expr);
}
std::any Resolver::visit(std::shared_ptr<Expr> curr){
    return curr->accept(*this);
}
void Resolver::resolve(std::shared_ptr<Stmt> stmt){
    visit(stmt);
}
void Resolver::resolve(std::vector<std::shared_ptr<Stmt>> statements){
    for (std::shared_ptr<Stmt> stmt : statements)
        resolve(stmt);
}
std::any Resolver::visit(std::shared_ptr<Stmt> curr){
    return curr->accept(*this);
}

// EXPR CHILD CLASSES
std::any Resolver::visitLiteral(std::shared_ptr<Literal> curr){
    // nothing to resolve
    return nullptr;
}
std::any Resolver::visitGrouping(std::shared_ptr<Grouping> curr){
    resolve(curr->expr);
    return nullptr;
}
std::any Resolver::visitUnary(std::shared_ptr<Unary> curr){
    resolve(curr->expr);
    return nullptr;
}
std::any Resolver::visitBinary(std::shared_ptr<Binary> curr){
    resolve(curr->left);
    resolve(curr->right);
    return nullptr;
}

std::any Resolver::visitVariable(std::shared_ptr<Variable> curr){
    // l-value of variable
    // eg. var a = a;
    // in this case, evaluating RHS leads to a being declared but not defined
    // an error is printed (not thrown) and execution will not proceed
    // otherwise, resolve local variable a
    if (!scopes.empty() && scopes.back().count(curr->name.lexeme) && scopes.back().at(curr->name.lexeme) == false)
        error(curr->name, "Can't read variable in its own initializer.");
    
    resolveLocal(curr, curr->name);
    return nullptr;
}
std::any Resolver::visitAssign(std::shared_ptr<Assign> curr){
    // resolve nested expression. then, resolve the whole assignment as a local variable
    resolve(curr->expr);
    resolveLocal(curr, curr->name);
    return nullptr;
}
std::any Resolver::visitLogical(std::shared_ptr<Logical> curr){
    // resolve expressions. no short-circuiting is done.
    resolve(curr->left);
    resolve(curr->right);
    return nullptr;
}

std::any Resolver::visitCall(std::shared_ptr<Call> curr){
    resolve(curr->callee);
    for (std::shared_ptr<Expr> arg : curr->arguments)
        resolve(arg);
    return nullptr;
}
std::any Resolver::visitGet(std::shared_ptr<Get> curr){
    resolve(curr->expr);
    return nullptr;
}
std::any Resolver::visitSet(std::shared_ptr<Set> curr){
    resolve(curr->expr);
    resolve(curr->value);
    return nullptr;
}

// STMT CHILD CLASSES
std::any Resolver::visitExpression(std::shared_ptr<Expression> curr){
    resolve(curr->expr);
    return nullptr;
}
std::any Resolver::visitPrint(std::shared_ptr<Print> curr){
    resolve(curr->expr);
    return nullptr;
}
std::any Resolver::visitVar(std::shared_ptr<Var> curr){
    // Variable declaration. Links with visitVariable(curr)
    declare(curr->name);
    if (curr->initializer)
        resolve(curr->initializer);
    define(curr->name);
    return nullptr;
}
std::any Resolver::visitBlock(std::shared_ptr<Block> curr){
    // create and resolve in new topmost scope. pop when done.
    beginScope();
    resolve(curr->statements);
    endScope();
    return nullptr;
}

std::any Resolver::visitIf(std::shared_ptr<If> curr){
    // resolve all branches.
    resolve(curr->condition);
    resolve(curr->thenBranch);
    if (curr->elseBranch) 
        resolve(curr->elseBranch);
    return nullptr;
}
std::any Resolver::visitWhile(std::shared_ptr<While> curr){
    resolve(curr->condition);
    resolve(curr->body);
    return nullptr;
}

std::any Resolver::visitFunction(std::shared_ptr<Function> curr){
    // resolve function name, then call helper method for arguments and body
    // resolveFunction will be reused for classes and methods
    declare(curr->name);
    define(curr->name);
    resolveFunction(curr, FunctionType::FUNCTION);
    return nullptr;
}
std::any Resolver::visitReturn(std::shared_ptr<Return> curr){
    // resolve return expression
    // the return keyword CANNOT show up in top-level code.
    // (DO NOT THROW)
    if (currentFunction == FunctionType::NONE)
        error(curr->keyword, "Cannot return from top-level code.").print();
    if(curr->expr) resolve(curr->expr);
    return nullptr;
}
std::any Resolver::visitClass(std::shared_ptr<Class> curr){
    declare(curr->name);
    define(curr->name);

    for (std::shared_ptr<Function> func : curr->methods){
        FunctionType type = FunctionType::METHOD;
        resolveFunction(func, type);
    }

    return nullptr;
}

LoxError::ParseError Resolver::error(Token token, std::string message){
    hasError = true;
    return LoxError::ParseError(token, message);
}

// ---HELPER FUNCTIONS---
void Resolver::beginScope(void){
    // create a new scope and push to stack
    scopes.push_back(std::unordered_map<std::string, bool>());
}
void Resolver::endScope(void){
    // pop the scope at top of stack
    scopes.pop_back();
}
void Resolver::declare(Token name){
    // declares a variable [name] in the topmost (current) scope by setting to false
    // redeclaration of local variable is a compilation error (DO NOT THROW)
    // redeclaration of global variable is not tracked by [scopes] and permitted
    if (scopes.empty()) return;
    if (scopes.back().count(name.lexeme))
        error(name, "Already a variable with this name in this scope.").print();
    scopes.back().insert({name.lexeme, false});
}
void Resolver::define(Token name){
    // defines a variable [name] in the topmost (current) scope by setting to true
    // declaration and definition are coupled in Lox,
    // so accessing a variable that is declared but not defined is a compilation error (DO NOT THROW)
    // redefinition can only happen with redeclaration and is thus not permitted
    // reassignment is treated separately from redefinition.
    if (scopes.empty()) return;
    scopes.back().at(name.lexeme) = true;
}
void Resolver::resolveLocal(std::shared_ptr<Expr> expr, Token name){
    // given a local variable [name], find the number of steps required
    // to resolve the variable to its scope
    // resolved variable are defined in its environment, evaluated line-by-line
    // store result as hash table
    for (int i = scopes.size() - 1; i >= 0; i--){
        if (scopes[i].count(name.lexeme)){
            interpreter.resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
    // variable exists in global scope. no resolution required.
}
void Resolver::resolveFunction(std::shared_ptr<Function> func, FunctionType type){
    // switches resolving type to given type, resolves arguments and body, then restores previous type

    const FunctionType enclosingType = currentFunction;
    currentFunction = type;

    beginScope();
    for (Token token : func->params){
        declare(token);
        define(token);
    }
    resolve(func->body);
    endScope();

    currentFunction = enclosingType;
}