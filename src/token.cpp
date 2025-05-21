#include "token.hpp"


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