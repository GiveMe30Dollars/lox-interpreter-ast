#include "lox.hpp"

void Lox::run(std::string source){
    Lox::hasError = false;

    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan();

    Parser parser(tokens);
    std::shared_ptr<Expr> expr = parser.parse();
    if (scanner.hasError || parser.hasError){
        hasError = true;
        return;
    }

    try{
        Object obj = interpreter.interpret(expr);
        std::cout << obj.toString(true) << "\n";
    }
    catch (LoxError::RuntimeError err){
        err.print();
        Lox::hasError = true;
        return;
    }
}