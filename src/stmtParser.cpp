
#include "stmtParser.hpp"

/*
program        → declaration* EOF ;

declaration    → classDecl
               | funcDecl
               | varDecl
               | statement ;

statement      → exprStmt
               | forStmt
               | ifStmt
               | printStmt
               | whileStmt
               | block ;

block          → "{" declaration* "}" ;
*/
/*
varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;
forStmt        → "for" "(" ( varDecl | exprStmt | ";" )
                 expression? ";"
                 expression? ")" statement ;
ifStmt         → "if" "(" expression ")" statement
               ( "else" statement )? ;
printStmt      → "print" expression ";" ;
exprDecl       → expression ";" ;
*/

std::vector<std::shared_ptr<Stmt>> StmtParser::parse(bool parseExpr){
    hasError = false;
    curr = 0;
    std::vector<std::shared_ptr<Stmt>> statements = {};
    while (!isAtEnd()){
        std::shared_ptr<Stmt> stmt = declaration();
        // only push non-empty pointers
        if (stmt) statements.push_back(stmt);
    }

    // expression mode: attempt to parse tokens as expression
    // if and only if hasError, parseExpr and no statements are parsed
    // if successful, encapsulate as print statement
    // TODO: catch and suppress the semicolon error into a warning
    if (parseExpr && hasError && statements.empty()){
        std::shared_ptr<Expr> expr = ExprParser::parse();
        if (!hasError) statements.push_back(std::make_shared<PrintStmt>(expr));
    }

    return statements;
}

// ---BASE STATEMENTS---
std::shared_ptr<Stmt> StmtParser::declaration(){
    try {
        if (match(Token::CLASS)) return classDeclaration();
        if (match(Token::FUN)) return functionDeclaration("function");
        if (match(Token::VAR)) return varDeclaration();
        return statement();
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
    return std::make_shared<VarStmt>(name, initializer);
}
std::shared_ptr<Stmt> StmtParser::statement(){
    if (match(Token::PRINT)) return printStatement();
    if (match(Token::LEFT_BRACE)) return std::make_shared<BlockStmt>(block());
    if (match(Token::IF)) return ifStatement();
    if (match(Token::WHILE)) return whileStatement();
    if (match(Token::FOR)) return forStatement();
    if (match(Token::RETURN)) return returnStatement();
    return exprStatement();
}
std::shared_ptr<Stmt> StmtParser::exprStatement(){
    std::shared_ptr<Expr> expr = expression();
    consume(Token::SEMICOLON, "Expect ';' after expression.");
    return std::make_shared<ExpressionStmt>(expr);
}
std::shared_ptr<Stmt> StmtParser::printStatement(){
    std::shared_ptr<Expr> expr = expression();
    consume(Token::SEMICOLON, "Expect ';' after value.");
    return std::make_shared<PrintStmt>(expr);
}
std::vector<std::shared_ptr<Stmt>> StmtParser::block(){
    std::vector<std::shared_ptr<Stmt>> statements = {};
    while (!check(Token::RIGHT_BRACE) && !isAtEnd()){
        std::shared_ptr<Stmt> stmt = declaration();
        // only push non-empty pointers
        if (stmt) statements.push_back(stmt);
    }
    consume(Token::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

// ---CONTROL FLOW---
std::shared_ptr<Stmt> StmtParser::ifStatement(){
    consume(Token::LEFT_PAREN, "Expect '(' after 'if'.");
    std::shared_ptr<Expr> condition = expression();
    consume(Token::RIGHT_PAREN, "Expect ')' after if condition.");

    // parse then branch. if else branch exists, parse that too.
    std::shared_ptr<Stmt> thenBranch = statement();
    std::shared_ptr<Stmt> elseBranch = nullptr;
    if (match(Token::ELSE)) elseBranch = statement();

    return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
}
std::shared_ptr<Stmt> StmtParser::whileStatement(){
    consume(Token::LEFT_PAREN, "Expect '(' after 'while'.");
    std::shared_ptr<Expr> condition = expression();
    consume(Token::RIGHT_PAREN, "Expect ')' after while condition.");
    std::shared_ptr<Stmt> body = statement();

    return std::make_shared<WhileStmt>(condition, body);
}
std::shared_ptr<Stmt> StmtParser::forStatement(){
    // desugaring. the for statement will be parsed as a while statement.
    // format:  for(initializer; condition; increment) body

    // parameters are delineated by SEMICOLON (initializer, condition)
    // and RIGHT_PAREN (incrementer)
    consume(Token::LEFT_PAREN, "Expect '(' after 'for'");

    // valid initializer is either none (;), a variable declaration or an expression statement
    std::shared_ptr<Stmt> initializer;
    if (match(Token::SEMICOLON)) initializer = nullptr;
    else if (match(Token::VAR)) initializer = varDeclaration();
    else initializer = exprStatement();

    // valid condition is either none (;) or an expression statement
    // the semicolon is consumed.
    std::shared_ptr<Expr> condition = nullptr;
    if (!check(Token::SEMICOLON)) condition = expression();
    consume(Token::SEMICOLON, "Expect ';' after loop condition.");

    // valid incrementer is either none () or an expression
    std::shared_ptr<Expr> increment = nullptr;
    if (!check(Token::RIGHT_PAREN)) increment = expression();
    consume(Token::RIGHT_PAREN, "Expect ')' after for clauses.");

    // body of for loop
    std::shared_ptr<Stmt> body = statement();

    // compilation of for loop to while loop:
    /*  {
            initializer;
            while (condition){
                body;
                increment;
            }
        } */

    // if there is an increment, enclose body and increment in new Block
    if (increment){
        std::vector<std::shared_ptr<Stmt>> v = {body, std::make_shared<ExpressionStmt>(increment)};
        body = std::make_shared<BlockStmt>(v);
    }

    // if there is no condition, assume while(true). create While.
    if (!condition) condition = std::make_shared<LiteralExpr>(Object::boolean(1));
    std::shared_ptr<Stmt> whileBlock = std::make_shared<WhileStmt>(condition, body);

    // if there is an initializer, enclose whileBlock and initializer in new Block
    if (initializer){
        std::vector<std::shared_ptr<Stmt>> v = {initializer, whileBlock};
        whileBlock = std::make_shared<BlockStmt>(v);
    }

    return whileBlock;
}

// ---FUNCTIONS AND CLASSES---
std::shared_ptr<FunctionStmt> StmtParser::functionDeclaration(std::string kind){
    Token name = consume(Token::IDENTIFIER, "Expect " + kind + " name.");
    consume(Token::LEFT_PAREN, "Expect '(' after " + kind + " name.");

    // parse parameters
    std::vector<Token> parameters = {};
    if (!check(Token::RIGHT_PAREN)){
        do{
            if (parameters.size() >= 255)
                error(peek(), "Can't have more than 255 arguments.");
            parameters.push_back(consume(Token::IDENTIFIER, "Expect variable name."));
        } while(match(Token::COMMA));
    }
    consume(Token::RIGHT_PAREN, "Expect ')' after parameters.");

    // block() expects the opening brace to be consumed
    consume(Token::LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<std::shared_ptr<Stmt>> body = block();

    return std::make_shared<FunctionStmt>(name, parameters, body);
}
std::shared_ptr<Stmt> StmtParser::returnStatement(){
    Token keyword = previous();
    std::shared_ptr<Expr> expr = nullptr;
    if (!check(Token::SEMICOLON)) expr = expression();
    consume(Token::SEMICOLON, "Expect ';' after return value.");

    return std::make_shared<ReturnStmt>(keyword, expr);
}
std::shared_ptr<Stmt> StmtParser::classDeclaration(){
    Token name = consume(Token::IDENTIFIER, "Expect class name.");
    consume(Token::LEFT_BRACE, "Expect '{' before class body");

    std::vector<std::shared_ptr<FunctionStmt>> methods = {};
    while (!isAtEnd() && !check(Token::RIGHT_BRACE)){
        methods.push_back(functionDeclaration("method"));
    }

    consume(Token::RIGHT_BRACE, "Expect '}' after class body.");

    return std::make_shared<ClassStmt>(name, methods);
}