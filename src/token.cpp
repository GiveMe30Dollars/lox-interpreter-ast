#include "token.hpp"

std::unordered_map<Token::TokenType,std::string> Token::tokenTypeName = {
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


bool isDigit(const char c){
    return (c >= '0' && c <= '9');
}
bool isAlpha (const char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
bool isAlphaNumeric (const char c){
    return isDigit(c) || isAlpha(c);
}

/*class Object {
    enum ObjectType {
        OBJECT_NIL, OBJECT_NUM, OBJECT_STR, OBJECT_BOOL
    };
    ObjectType type;
    std::string literalString;
    double literalNumber;
    ...
*/

Object Object::nil(){
    Object obj;
    obj.type = Object::NIL;
    return obj;
}
Object Object::boolean(bool b){
    Object obj;
    obj.type = Object::BOOL;
    obj.literalBool = b;
    return obj;
}
Object Object::number(double val){
    Object obj;
    obj.type = Object::NUMBER;
    obj.literalNumber = val;
    return obj;
}
Object Object::string(std::string s){
    Object obj;
    obj.type = Object::STRING;
    obj.literalString = s;
    return obj;
}
std::string Object::toString(bool useLox){
    switch(type){
        case Object::NIL: return useLox ? "nil" : "null";
        case Object::BOOL: return literalBool ? "true" : "false";
        case Object::NUMBER:
            // if number is integer, cast to int, then to string and add ".0"
            // otherwise cast to string directly and strip trailing 0s
            // this ensures all numbers are displayed in at least 1 d.p.
            if (floor(literalNumber) == literalNumber){
                return (std::to_string((int)literalNumber) + ".0");
            }
            else {
                std::string str = std::to_string(literalNumber);
                str.erase(str.find_last_not_of('0') + 1, std::string::npos);
                return str;
            }
        case Object::STRING: 
            return literalString;

        default:
            throw "UNIMPLEMENTED toString type!";
            return "";
    }
    return "";
}

/*
template<>
Object Object::cast<Object::BOOL>(){
    return Object::boolean(isTruthy());
}

template<>
Object Object::cast<Object::NUMBER>(){
    switch (type){
        case Object::NIL:
            return Object::number(0);
        case Object::BOOL:
            return Object::number(literalBool ? 1 : 0);
        case Object::NUMBER: 
            return *this;
        case Object::STRING:
        {
            try {
                double val = std::stod(literalString);
                return Object::number(val);
            }
            catch(std::invalid_argument& e) {
                return Object::number(0);
            }
        }
    }
}

template<Object::ObjectType T>
Object Object::cast(){
    throw ("Unimplemeneted cast type!");
    return nil();
}
*/



/*class Token {
    private:
        TokenType type;
        std::string lexeme;
        std::string literal;
        int line;
};*/

std::string Token::toString() {
    return tokenTypeName.at(type) + " " + lexeme + " " + literal.toString();
}