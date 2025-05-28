// base class, from which all others inherit
// requires stings, vectors, sets, maps and cmath
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cmath>

// required for smart pointers
#include <memory>

#pragma once

// LoxCallable, LoxClass and LoxInstance are included in the .cpp file and
// are SEPARATELY DECLARED defined in the .hpp file.
// this is to avoid circular dependencies.
// As Object methods are not defined in header files, and are only needed for smart pointers
// no implementation details of the classes themselves is required.
class LoxCallable;
//class LoxClass;
//class LoxInstance;

/*
    HEADER FILES SHOUD DECLARE:
        PUBLIC variables
        PUBLIC functions
        PUBLIC classes
            This includes public and private variables,
            public and private functions,
            and main and delegated constructors.
    Declarations include variable names, functions and their parameters.
*/


bool isDigit(const char c);
bool isAlpha(const char c);
bool isAlphaNumeric(const char c);

class Object {
    // Class to represent an arbitrary literal
    // Pass by value for all subsequent use
    public:
        enum ObjectType {
            NIL, NUMBER, STRING, BOOL,
            LOX_CALLABLE
        };
        ObjectType type;
        bool literalBool;
        double literalNumber;
        std::string literalString;
        std::shared_ptr<LoxCallable> loxCallable;
        std::string toString(bool useLox = false);
        
        // Generator functions
        static Object nil(void);
        static Object boolean(bool b);
        static Object number(double val);
        static Object string(std::string str);
        static Object function(std::shared_ptr<LoxCallable> callable);

        /*
        template<Object::ObjectType T>
        Object cast();
        */

};

class Token {
    // Class to represent a token
    // Pass by value for all subsequent use
    public:
        enum TokenType {
            // Single-character tokens.
            LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
            COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

            // One or two character tokens.
            BANG, BANG_EQUAL,
            EQUAL, EQUAL_EQUAL,
            GREATER, GREATER_EQUAL,
            LESS, LESS_EQUAL,

            // Literals.
            IDENTIFIER, STRING, NUMBER,

            // Keywords.
            AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
            PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

            _EOF
        };

        TokenType type;
        std::string lexeme;
        Object literal;
        int line;
        Token(TokenType type, std::string lexeme, Object literal, int line) :
            type(type), lexeme(lexeme), literal(literal), line(line) {}
        std::string toString(void);
        static std::unordered_map<TokenType,std::string> tokenTypeName;
};