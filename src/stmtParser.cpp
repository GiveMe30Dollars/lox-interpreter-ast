
#include "stmtParser.hpp"

/*
program        → declaration* EOF ;

declaration    → varDecl
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
        if (!hasError) statements.push_back(std::make_shared<Print>(expr));
    }

    return statements;
}

// ---BASE STATEMENTS---
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
    if (match(Token::IF)) return ifStatement();
    if (match(Token::LEFT_BRACE)) return block();
    if (match(Token::WHILE)) return whileStatement();
    if (match(Token::FOR)) return forStatement();
    return exprStatement();
}
std::shared_ptr<Stmt> StmtParser::exprStatement(){
    std::shared_ptr<Expr> expr = expression();
    consume(Token::SEMICOLON, "Expect ';' after expression.");
    return std::make_shared<Expression>(expr);
}
std::shared_ptr<Stmt> StmtParser::printStatement(){
    std::shared_ptr<Expr> expr = expression();
    consume(Token::SEMICOLON, "Expect ';' after value.");
    return std::make_shared<Print>(expr);
}
std::shared_ptr<Stmt> StmtParser::block(){
    std::vector<std::shared_ptr<Stmt>> statements = {};
    while (!check(Token::RIGHT_BRACE) && !isAtEnd()){
        std::shared_ptr<Stmt> stmt = declaration();
        // only push non-empty pointers
        if (stmt) statements.push_back(stmt);
    }
    consume(Token::RIGHT_BRACE, "Expect '}' after block.");
    return std::make_shared<Block>(statements);
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

    return std::make_shared<If>(condition, thenBranch, elseBranch);
}
std::shared_ptr<Stmt> StmtParser::whileStatement(){
    consume(Token::LEFT_PAREN, "Expect '(' after 'while'.");
    std::shared_ptr<Expr> condition = expression();
    consume(Token::RIGHT_PAREN, "Expect ')' after while condition.");
    std::shared_ptr<Stmt> body = statement();

    return std::make_shared<While>(condition, body);
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
        std::vector<std::shared_ptr<Stmt>> v = {body, std::make_shared<Expression>(increment)};
        body = std::make_shared<Block>(v);
    }

    // if there is no condition, assume while(true). create While.
    if (!condition) condition = std::make_shared<Literal>(Object::boolean(1));
    std::shared_ptr<Stmt> whileBlock = std::make_shared<While>(condition, body);

    // if there is an initializer, enclose whileBlock and initializer in new Block
    if (initializer){
        std::vector<std::shared_ptr<Stmt>> v = {initializer, whileBlock};
        whileBlock = std::make_shared<Block>(v);
    }

    return whileBlock;
}