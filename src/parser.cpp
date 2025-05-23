#include "parser.hpp"

bool Parser::isAtEnd(){
    return tokens.at(curr).type == Token::_EOF;
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

bool Parser::check(Token::TokenType t){
    if (isAtEnd()) return false;
    return peek().type == t;
}

template<typename... Args>
bool Parser::match(Args... args){
    // check if any of the TokenTypes given matches the current token
    // if yes, advance and return true
    for (const Token::TokenType t : {args...}){
        if (peek().type == t){
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(Token::TokenType t, std::string err){
    // consumes current token of TokenType t
    // if current token is not t, throw an error
    if (check(t)) return advance();
    throw(error(peek(), err));
}

LoxError::ParseError Parser::error(Token token, std::string err){
    hasError = true;
    return LoxError::ParseError(token, err);
}

void Parser::synchronize(){
    advance();
    while (!isAtEnd()){
        if (previous().type == Token::SEMICOLON) return;
        switch (peek().type){
            case Token::CLASS:
            case Token::FUN:
            case Token::VAR:
            case Token::FOR:
            case Token::IF:
            case Token::WHILE:
            case Token::PRINT:
            case Token::RETURN:
                return;
        }
        advance();
    }
}


std::shared_ptr<Expr> Parser::parse(){
    try{
        return expression();
    }
    catch(LoxError::ParseError err){
        err.print();
        synchronize();
        parse();
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
    while (match(Token::BANG_EQUAL, Token::EQUAL_EQUAL)){
        Token op = previous();
        std::shared_ptr<Expr> right = comparison();
        std::cerr << "made other!\n";
        expr = std::make_shared<Binary>(expr, op, right);
        std::cerr << "merged with other!\n";
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison(){
    std::shared_ptr<Expr> expr = term();
    while (match(Token::GREATER, Token::GREATER_EQUAL, Token::LESS, Token::LESS_EQUAL)){
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::term(){
    std::shared_ptr<Expr> expr = factor();
    while (match(Token::MINUS, Token::PLUS)){
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::factor(){
    std::shared_ptr<Expr> expr = unary();
    while (match(Token::STAR, Token::SLASH)){
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::unary(){
    if (match(Token::BANG, Token::MINUS)){
        Token op = previous();
        std::shared_ptr<Expr> expr = unary();
        return std::make_shared<Unary>(op, expr);
    }
    return primary();
}

std::shared_ptr<Expr> Parser::primary(){
    if (match(Token::TRUE)) return std::make_shared<Literal>(Object::boolean(1));
    if (match(Token::FALSE)) return std::make_shared<Literal>(Object::boolean(0));
    if (match(Token::NIL)) return std::make_shared<Literal>(Object::nil());

    if (match(Token::NUMBER, Token::STRING)) 
        return std::make_shared<Literal>(previous().literal);
    
    if (match(Token::LEFT_PAREN)){
        std::shared_ptr<Expr> expr = expression();
        consume(Token::RIGHT_PAREN, "Expect ) after expression.");
        return std::make_shared<Grouping>(expr);
    }

    // at end of file. return
    if (isAtEnd()) return nullptr;

    // invalid token.
    throw(error(peek(), "Expect expression."));
}
