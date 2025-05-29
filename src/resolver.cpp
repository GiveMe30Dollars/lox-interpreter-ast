#include "resolver.hpp"

// requires implemetation details of Interpreter
// (previously declared separately)
#include "interpreter.hpp"


Resolver::Resolver(Interpreter& interpreter) : interpreter(interpreter){
    // interpreter has to be passed as member initializer
    hasError = false;
    scopes = {};
    currentFunction = FunctionType::NONE;
    currentClass = ClassType::NONE;
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
std::any Resolver::visitLiteralExpr(std::shared_ptr<LiteralExpr> curr){
    // nothing to resolve
    return nullptr;
}
std::any Resolver::visitGroupingExpr(std::shared_ptr<GroupingExpr> curr){
    resolve(curr->expr);
    return nullptr;
}
std::any Resolver::visitUnaryExpr(std::shared_ptr<UnaryExpr> curr){
    resolve(curr->expr);
    return nullptr;
}
std::any Resolver::visitBinaryExpr(std::shared_ptr<BinaryExpr> curr){
    resolve(curr->left);
    resolve(curr->right);
    return nullptr;
}

std::any Resolver::visitVariableExpr(std::shared_ptr<VariableExpr> curr){
    // l-value of variable
    // eg. var a = a;
    // in this case, evaluating RHS leads to a being declared but not defined
    // an error is printed (not thrown) and execution will not proceed
    // otherwise, resolve local variable a
    if (!scopes.empty() && scopes.back().count(curr->name.lexeme) && scopes.back().at(curr->name.lexeme) == false)
        error(curr->name, "Cannot read variable in its own initializer.");
    
    resolveLocal(curr, curr->name);
    return nullptr;
}
std::any Resolver::visitAssignExpr(std::shared_ptr<AssignExpr> curr){
    // resolve nested expression. then, resolve the whole assignment as a local variable
    resolve(curr->expr);
    resolveLocal(curr, curr->name);
    return nullptr;
}
std::any Resolver::visitLogicalExpr(std::shared_ptr<LogicalExpr> curr){
    // resolve expressions. no short-circuiting is done.
    resolve(curr->left);
    resolve(curr->right);
    return nullptr;
}

std::any Resolver::visitCallExpr(std::shared_ptr<CallExpr> curr){
    resolve(curr->callee);
    for (std::shared_ptr<Expr> arg : curr->arguments)
        resolve(arg);
    return nullptr;
}
std::any Resolver::visitGetExpr(std::shared_ptr<GetExpr> curr){
    resolve(curr->expr);
    return nullptr;
}
std::any Resolver::visitSetExpr(std::shared_ptr<SetExpr> curr){
    resolve(curr->expr);
    resolve(curr->value);
    return nullptr;
}
std::any Resolver::visitThisExpr(std::shared_ptr<ThisExpr> curr){
    if (currentClass == ClassType::NONE){
        error(curr->keyword, "Cannot use 'this' outside a class.");
        return nullptr;
    }
    resolveLocal(curr, curr->keyword);
    return nullptr;
}
std::any Resolver::visitSuperExpr(std::shared_ptr<SuperExpr> curr){
    resolveLocal(curr, curr->keyword);
    return nullptr;
}


// STMT CHILD CLASSES
std::any Resolver::visitExpressionStmt(std::shared_ptr<ExpressionStmt> curr){
    resolve(curr->expr);
    return nullptr;
}
std::any Resolver::visitPrintStmt(std::shared_ptr<PrintStmt> curr){
    resolve(curr->expr);
    return nullptr;
}
std::any Resolver::visitVarStmt(std::shared_ptr<VarStmt> curr){
    // Variable declaration. Links with visitVariable(curr)
    declare(curr->name);
    if (curr->initializer)
        resolve(curr->initializer);
    define(curr->name);
    return nullptr;
}
std::any Resolver::visitBlockStmt(std::shared_ptr<BlockStmt> curr){
    // create and resolve in new topmost scope. pop when done.
    beginScope();
    resolve(curr->statements);
    endScope();
    return nullptr;
}

std::any Resolver::visitIfStmt(std::shared_ptr<IfStmt> curr){
    // resolve all branches.
    resolve(curr->condition);
    resolve(curr->thenBranch);
    if (curr->elseBranch) 
        resolve(curr->elseBranch);
    return nullptr;
}
std::any Resolver::visitWhileStmt(std::shared_ptr<WhileStmt> curr){
    resolve(curr->condition);
    resolve(curr->body);
    return nullptr;
}

std::any Resolver::visitFunctionStmt(std::shared_ptr<FunctionStmt> curr){
    // resolve function name, then call helper method for arguments and body
    // resolveFunction will be reused for classes and methods
    declare(curr->name);
    define(curr->name);
    resolveFunction(curr, FunctionType::FUNCTION);
    return nullptr;
}
std::any Resolver::visitReturnStmt(std::shared_ptr<ReturnStmt> curr){
    // resolve return expression
    // 'return' CANNOT show up in top-level code.
    // return cannot be non-NIL if in initializer
    // (DO NOT THROW)
    if (currentFunction == FunctionType::NONE)
        error(curr->keyword, "Cannot return from top-level code.").print();
    if(curr->expr){
        if (currentFunction == FunctionType::INITIALIZER)
            error(curr->keyword, "Cannot return a value from initializer.").print();
        resolve(curr->expr);
    }
    return nullptr;
}
std::any Resolver::visitClassStmt(std::shared_ptr<ClassStmt> curr){
    // declare and define class name
    // then, in new scope, define this keyword and all methods
    const ClassType enclosingType = currentClass;
    currentClass = ClassType::CLASS;

    declare(curr->name);
    define(curr->name);
    if (curr->superclass){
        if (curr->superclass->name.lexeme == curr->name.lexeme)
            error(curr->superclass->name, "A class cannot inherit from itself.");
        resolve(curr->superclass);

        // add 'super' for this class in an enclosing scope
        beginScope();
        scopes.back().insert({"super", true});
    }

    beginScope();
    scopes.back().insert({"this", true});
    for (std::shared_ptr<FunctionStmt> func : curr->methods){
        FunctionType type = FunctionType::METHOD;
        if (func->name.lexeme == "init")
            type = FunctionType::INITIALIZER;
        resolveFunction(func, type);
    }
    endScope();

    // end enclosing scope if inheriting from superclass
    if (curr->superclass) endScope();

    currentClass = enclosingType;

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
void Resolver::resolveFunction(std::shared_ptr<FunctionStmt> func, FunctionType type){
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