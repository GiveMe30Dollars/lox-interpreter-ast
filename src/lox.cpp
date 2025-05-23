#include "lox.hpp"

bool Lox::hasCompileError = false;
bool Lox::hasRuntimeError = false;
Interpreter Lox::interpreter;

void Lox::run(std::string source){
    Lox::hasCompileError = false;
    Lox::hasRuntimeError = false;

    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan();

    Parser parser(tokens);
    std::shared_ptr<Expr> expr = parser.parse();
    if (scanner.hasError || parser.hasError){
        hasCompileError = true;
        return;
    }
    ASTPrinter printer;
    std::cerr << printer.print(expr) << "\n";

    try{
        Object obj = interpreter.interpret(expr);

        // workaround for CodeCrafters testcase: remove when independent
        if (obj.type == Object::NUMBER){
            double val = obj.literalNumber;
            if (floor(val) == val){
                std::cout << std::to_string((int)val) << "\n";
                return;
            }
        }
        std::cout << obj.toString(true) << "\n";
    }
    catch (LoxError::RuntimeError err){
        err.print();
        Lox::hasRuntimeError = true;
        return;
    }
}