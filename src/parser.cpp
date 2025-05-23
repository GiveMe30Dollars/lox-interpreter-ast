#include "parser.hpp"

bool Parser::isAtEnd(){
    return tokens.at(curr).type == TokenType::_EOF;
}

Token Parser::advance(){
    if (!isAtEnd()) curr++;
    return previous();
}

Token Parser::peek(){
    return tokens.at(curr);
}

Token Parser::previous(){
    return tokens.at(curr - 1);
}

bool Parser::check(TokenType t){
    if (isAtEnd()) return false;
    return peek().type == t;
}

template<typename... Args>
bool Parser::match(Args... args){
    // check if any of the TokenTypes given matches the current token
    // if yes, advance and return true
    for (const TokenType t : {args...}){
        if (peek().type == t){
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType t, std::string err){
    // consumes current token of TokenType t
    // if current token is not t, throw an error
    if (check(t)) return advance();
    throw(error(peek(), err));
}

Parser::ParseError Parser::error(Token token, std::string err){
    hasError = true;
    LoxError::print(token, err);
}


std::shared_ptr<Expr> Parser::parse(){
    try{
        return expression();
    }
    catch(ParseError err){
        return nullptr;
    }
}


//   ---BASIC PARSING RULES---
/*
expression     → equality ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" ;
*/

std::shared_ptr<Expr> Parser::expression(){
    return equality();
}

std::shared_ptr<Expr> Parser::equality(){
    std::shared_ptr<Expr> expr = comparison();
    while (match(BANG_EQUAL, EQUAL_EQUAL)){
        Token op = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison(){
    std::shared_ptr<Expr> expr = term();
    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)){
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::term(){
    std::shared_ptr<Expr> expr = factor();
    while (match(MINUS, PLUS)){
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::factor(){
    std::shared_ptr<Expr> expr = unary();
    while (match(STAR, SLASH)){
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::unary(){
    if (match(BANG, MINUS)){
        Token op = previous();
        std::shared_ptr<Expr> expr = unary();
        return std::make_shared<Unary>(op, expr);
    }
    return primary();
}

std::shared_ptr<Expr> Parser::primary(){
    if (match(TRUE)) return std::make_shared<Literal>(Object::objBool(1));
    if (match(FALSE)) return std::make_shared<Literal>(Object::objBool(0));
    if (match(NIL)) return std::make_shared<Literal>(Object::objNil());

    if (match(NUMBER, STRING)) 
        return std::make_shared<Literal>(previous().literal);
    
    if (match(LEFT_PAREN)){
        std::shared_ptr<Expr> expr = expression();
        consume(RIGHT_PAREN, "Expect ) after expression.");
        return std::make_shared<Grouping>(expr);
    }

    // at end of file. return
    if (isAtEnd()) return nullptr;

    // invalid token.
    throw(error(peek(), "Expect expression."));
}
