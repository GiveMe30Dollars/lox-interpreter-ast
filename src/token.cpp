#include "token.hpp"

std::unordered_map<TokenType,std::string> Token::tokenTypeName = {
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
Object Object::objNil(){
    Object obj;
    obj.type = OBJECT_NIL;
    return obj;
}
Object Object::objBool(bool b){
    Object obj;
    obj.type = OBJECT_BOOL;
    obj.literalBool = b;
    return obj;
}
Object Object::objNum(double val){
    Object obj;
    obj.type = OBJECT_NUM;
    obj.literalNumber = val;
    return obj;
}
Object Object::objStr(std::string s){
    Object obj;
    obj.type = OBJECT_STR;
    obj.literalString = s;
    return obj;
}
std::string Object::toString(){
    switch(type){
        case OBJECT_NIL: return "null";
        case OBJECT_BOOL: return literalBool ? "true" : "false";
        case OBJECT_NUM:
            if (floor(literalNumber) == literalNumber) return (std::to_string((int)floor(literalNumber)) + ".0");
            else return std::to_string(literalNumber);
        case OBJECT_STR: 
            return literalString;

        default:
            std::cerr << "UNIMPLEMENTED toString type!";
            return "";
    }
    return "";
}


/*class Token {
    private:
        TokenType type;
        std::string lexeme;
        std::string literal;
        int line;
};*/

Token::Token(TokenType type, std::string lexeme, Object literal, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
}

std::string Token::toString() {
    return tokenTypeName.at(type) + " " + lexeme + " " + literal.toString();
}