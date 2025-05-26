
#include "stmtParser.hpp"
#include "ASTPrinter.hpp"

/*
program        → declaration* EOF ;

declaration    → varDecl
               | statement ;

statement      → exprStmt
               | printStmt ;
*/
/*
varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;
printStmt      → "print" expression ";" ;
exprDecl       → expression ";" ;
*/

std::vector<std::shared_ptr<Stmt>> StmtParser::parse(bool parseExpr){
    hasError = false;
    curr = 0;
    std::vector<std::shared_ptr<Stmt>> statements = {};
    while (!isAtEnd()){
        std::shared_ptr<Stmt> stmt = declaration();
        if (stmt != nullptr) statements.push_back(stmt);
    }

    // expression mode: attempt to parse tokens as expression
    // if and only if hasError, parseExpr and no statements are parsed
    // if successful, encapsulate as print statement
    if (parseExpr && hasError && statements.empty()){
        std::shared_ptr<Expr> expr = ExprParser::parse();
        if (!hasError) statements.push_back(std::make_shared<Print>(expr));
    }

    return statements;
}

std::shared_ptr<Stmt> StmtParser::declaration(){
    try {
        if (match(Token::VAR)) return varDeclaration();
        else return statement();
    }
    catch(LoxError::ParseError err){
        // synchronize and return.
        err.print();
        synchronize();
        return nullptr;
    }
}
std::shared_ptr<Stmt> StmtParser::varDeclaration(){
    Token name = consume(Token::IDENTIFIER, "Expect variable name.");
    std::shared_ptr<Expr> initializer = nullptr;
    if (match(Token::EQUAL)){
        initializer = expression();
    }
    consume(Token::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<Var>(name, initializer);
}
std::shared_ptr<Stmt> StmtParser::statement(){
    if (match(Token::PRINT)) return printStatement();
    else return exprStatement();
}
std::shared_ptr<Stmt> StmtParser::exprStatement(){
    std::shared_ptr<Expr> expr = expression();
    consume(Token::SEMICOLON, "Expect ';' after expression.");
    return std::make_shared<Expression>(expr);
}
std::shared_ptr<Stmt> StmtParser::printStatement(){
    std::shared_ptr<Expr> expr = expression();
    consume(Token::SEMICOLON, "Expect ';' after value.");
    ASTPrinter printer;
    std::shared_ptr<Print> stmt = std::make_shared<Print>(expr);
    std::cerr << "creation successful\n";
    std::cerr << printer.print(stmt) << "\n";
    return std::make_shared<Print>(expr);
}