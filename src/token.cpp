#include "token.hpp"

const std::unordered_map<TokenType,std::string> tokenTypeName = {
    {LEFT_PAREN, "LEFT_PAREN"}, 
    {RIGHT_PAREN, "RIGHT_PAREN"},
    {LEFT_BRACE, "LEFT_BRACE"}, 
    {RIGHT_BRACE, "RIGHT_BRACE"},

    {COMMA, "COMMA"}, 
    {DOT, "DOT"}, 
    {MINUS, "MINUS"}, 
    {PLUS, "PLUS"},
    {SEMICOLON, "SEMICOLON"}, 
    {SLASH, "SLASH"}, 
    {STAR, "STAR"},

    {BANG, "BANG"}, 
    {BANG_EQUAL, "BANG_EQUAL"}, 
    {EQUAL, "EQUAL"}, 
    {EQUAL_EQUAL, "EQUAL_EQUAL"},
    {GREATER, "GREATER"}, 
    {GREATER_EQUAL, "GREATER_EQUAL"},
    {LESS, "LESS"}, 
    {LESS_EQUAL, "LESS_EQUAL"},

    {IDENTIFIER, "IDENTIFIER"}, 
    {STRING, "STRING"}, 
    {NUMBER, "NUMBER"},

    {AND,"AND"}, 
    {CLASS,"CLASS"}, 
    {ELSE,"ELSE"}, 
    {FALSE,"FALSE"},
    {FUN,"FUN"}, 
    {FOR,"FOR"}, 
    {IF,"IF"}, 
    {NIL,"NIL"}, 
    {OR,"OR"},
    {PRINT,"PRINT"}, 
    {RETURN,"RETURN"}, 
    {SUPER,"SUPER"}, 
    {THIS,"THIS"},
    {TRUE,"TRUE"}, 
    {VAR,"VAR"}, 
    {WHILE,"WHILE"},
    
    {_EOF, "EOF"}
};

const std::unordered_map<std::string, TokenType> reservedKeywords = {
    {"and", AND},
    {"class", CLASS},
    {"else", ELSE},
    {"false", FALSE},
    {"fun", FUN},
    {"for", FOR},
    {"if", IF},
    {"nil", NIL},
    {"or", OR},
    {"print", PRINT},
    {"return", RETURN},
    {"super", SUPER},
    {"this", THIS},
    {"true", TRUE},
    {"var", VAR},
    {"while", WHILE},
};


bool isDigit(const char c){
    return (c >= '0' && c <= '9');
}
bool isAlpha (const char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
bool isAlphaNumeric (const char c){
    return isDigit(c) || isAlpha(c);
}

/*class Token {
    private:
        TokenType type;
        std::string lexeme;
        std::string literal;
        int line;
};*/

Token::Token(TokenType type, std::string lexeme, std::string literalString, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literalString = literalString;
    this->line = line;
}
Token::Token(TokenType type, std::string lexeme, std::string literal, double literalNumber, int line) : Token(type, lexeme, literal, line){
    this->literalNumber = literalNumber;
}

std::string Token::toString() {
    return tokenTypeName.at(type) + " " + lexeme + " " + literalString;
}