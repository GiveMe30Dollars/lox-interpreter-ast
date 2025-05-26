#include "lox.hpp"

bool Lox::hasCompileError = false;
bool Lox::hasRuntimeError = false;
Interpreter Lox::interpreter;

void Lox::run(std::string source, bool parseExpr){
    Lox::hasCompileError = false;
    Lox::hasRuntimeError = false;

    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan();

    StmtParser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse(parseExpr);
    if (scanner.hasError || parser.hasError){
        hasCompileError = true;
        return;
    }
    //for (std::shared_ptr<Stmt> stmt : statements) std::cerr << ASTPrinter::print(stmt) << "\n";

    try{
        interpreter.execute(statements);
    }
    catch (LoxError::RuntimeError err){
        err.print();
        Lox::hasRuntimeError = true;
        return;
    }
}