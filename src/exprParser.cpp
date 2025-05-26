#include "exprParser.hpp"

bool ExprParser::isAtEnd(){
    return tokens.at(curr).type == Token::_EOF;
}

Token ExprParser::advance(){
    if (!isAtEnd()) curr++;
    return previous();
}

Token ExprParser::peek(){
    return tokens.at(curr);
}

Token ExprParser::previous(){
    return tokens.at(curr - 1);
}

bool ExprParser::check(Token::TokenType t){
    if (isAtEnd()) return false;
    return peek().type == t;
}

template<typename... Args>
bool ExprParser::match(Args... args){
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

Token ExprParser::consume(Token::TokenType t, std::string err){
    // consumes current token of TokenType t
    // if current token is not t, throw an error
    if (check(t)) return advance();
    throw(error(peek(), err));
}

LoxError::ParseError ExprParser::error(Token token, std::string err){
    hasError = true;
    return LoxError::ParseError(token, err);
}

void ExprParser::synchronize(){
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


std::shared_ptr<Expr> ExprParser::parse(){
    hasError = false;
    curr = 0;
    try{
        return expression();
    }
    catch(LoxError::ParseError err){
        err.print();
        return nullptr;
    }
}


//   ---BASIC PARSING RULES---
/*
expression     → assignment ;
assignment     → IDENTIFIER "=" assignment
               | equality ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" ;
*/

std::shared_ptr<Expr> ExprParser::expression(){
    return equality();
}

std::shared_ptr<Expr> ExprParser::assignment(){
    throw "UNIMPLEMENTED assignment in ExprParser!";
    return equality();
}

std::shared_ptr<Expr> ExprParser::equality(){
    std::shared_ptr<Expr> expr = comparison();
    while (match(Token::BANG_EQUAL, Token::EQUAL_EQUAL)){
        Token op = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> ExprParser::comparison(){
    std::shared_ptr<Expr> expr = term();
    while (match(Token::GREATER, Token::GREATER_EQUAL, Token::LESS, Token::LESS_EQUAL)){
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> ExprParser::term(){
    std::shared_ptr<Expr> expr = factor();
    while (match(Token::MINUS, Token::PLUS)){
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> ExprParser::factor(){
    std::shared_ptr<Expr> expr = unary();
    while (match(Token::STAR, Token::SLASH)){
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> ExprParser::unary(){
    if (match(Token::BANG, Token::MINUS)){
        Token op = previous();
        std::shared_ptr<Expr> expr = unary();
        return std::make_shared<Unary>(op, expr);
    }
    return primary();
}

std::shared_ptr<Expr> ExprParser::primary(){
    // true, false, nil literals
    if (match(Token::TRUE)) return std::make_shared<Literal>(Object::boolean(1));
    if (match(Token::FALSE)) return std::make_shared<Literal>(Object::boolean(0));
    if (match(Token::NIL)) return std::make_shared<Literal>(Object::nil());

    // number and string literals
    if (match(Token::NUMBER, Token::STRING)) 
        return std::make_shared<Literal>(previous().literal);
    
    // grouping (parenthesis pair)
    if (match(Token::LEFT_PAREN)){
        std::shared_ptr<Expr> expr = expression();
        consume(Token::RIGHT_PAREN, "Expect ) after expression.");
        return std::make_shared<Grouping>(expr);
    }

    // identifier
    if (match(Token::IDENTIFIER))
        return std::make_shared<Variable>(previous());

    // at end of file. return
    if (isAtEnd()) return nullptr;

    // invalid token.
    throw(error(peek(), "Expect expression."));
}
