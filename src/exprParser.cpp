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


//   ---PARSING RULES---
/*
expression     → assignment ;
assignment     → IDENTIFIER "=" assignment
               | logic_or ;
logic_or       → logic_and ( "or" logic_and )* ;
logic_and      → equality ( "and" equality )* ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary | call ;
call           → primary ( "(" arguments? ")" )* ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" | IDENTIFIER ;
*/

std::shared_ptr<Expr> ExprParser::expression(){
    return assignment();
}

std::shared_ptr<Expr> ExprParser::assignment(){
    std::shared_ptr<Expr> expr = logicOr();
    // if the next token is EQUAL, parse as assignment
    // (expr must be a variable l-value)
    // otherwise, parse (beforehand) and return as equality
    if (match(Token::EQUAL)){
        Token op = previous();
        std::shared_ptr<Expr> value = assignment();
        if (Variable* e = dynamic_cast<Variable*>(expr.get())){
            Token name = e->name;
            return std::make_shared<Assign>(name, value);
        }
        else if (Get* e = dynamic_cast<Get*>(expr.get())){
            return std::make_shared<Set>(e->expr, e->name, value);
        }
        else throw error(op, "Invalid assignment target.");
    }
    else return expr;
}

std::shared_ptr<Expr> ExprParser::logicOr(){
    std::shared_ptr<Expr> expr = logicAnd();
    while (match(Token::OR)){
        Token op = previous();
        std::shared_ptr<Expr> right = logicAnd();
        expr = std::make_shared<Logical>(expr, op, right);
    }
    return expr;
}
std::shared_ptr<Expr> ExprParser::logicAnd(){
    std::shared_ptr<Expr> expr = equality();
    while (match(Token::AND)){
        Token op = previous();
        std::shared_ptr<Expr> right = equality();
        expr = std::make_shared<Logical>(expr, op, right);
    }
    return expr;
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
    return call();
}

std::shared_ptr<Expr> ExprParser::call(){
    std::shared_ptr<Expr> expr = primary();
    while (true){
        if (match(Token::LEFT_PAREN)){
            expr = finishCall(expr);
        }
        else if (match(Token::DOT)){
            Token name = consume(Token::IDENTIFIER, "Expect property name after '.'");
            expr = std::make_shared<Get>(expr, name);
        }
        else break;
    }
    return expr;
}
std::shared_ptr<Expr> ExprParser::finishCall(std::shared_ptr<Expr> callee){
    std::vector<std::shared_ptr<Expr>> arguments = {};
    if (!check(Token::RIGHT_PAREN)){
        do{
            if (arguments.size() >= 255)
                error(peek(), "can't have more than 255 arguments.");
            arguments.push_back(expression());
        } while (match(Token::COMMA));
    }
    Token paren = consume(Token::RIGHT_PAREN, "Expect ')' after arguments.");
    return std::make_shared<Call>(callee, paren, arguments);
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

    // identifier (this, generic identifier)
    if (match(Token::THIS))
        return std::make_shared<This>(previous());
    if (match(Token::IDENTIFIER))
        return std::make_shared<Variable>(previous());

    // at end of file. return
    if (isAtEnd()) return nullptr;

    // invalid token.
    throw(error(peek(), "Expect expression."));
}
