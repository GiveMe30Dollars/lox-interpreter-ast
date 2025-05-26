#include "lox.hpp"

bool Lox::hasCompileError = false;
bool Lox::hasRuntimeError = false;
Interpreter Lox::interpreter;

void Lox::run(std::string source, bool parseExpr){
    Lox::hasCompileError = false;
    Lox::hasRuntimeError = false;

    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan();
    for (Token t : tokens) std::cerr << t.toString() << "\n";

    StmtParser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse(parseExpr);
    if (scanner.hasError || parser.hasError){
        hasCompileError = true;
        return;
    }

    ASTPrinter printer;
    std::cerr << std::to_string(statements.size()) << "\n";
    for (std::shared_ptr<Stmt> stmt : statements){
        if (stmt == nullptr){
            std::cerr << "HUHHH\n";
            continue;
        }
        else std::cerr << printer.print(stmt) << "\n";
    }

    try{
        interpreter.interpret(statements);
    }
    catch (LoxError::RuntimeError err){
        err.print();
        Lox::hasRuntimeError = true;
        return;
    }
}