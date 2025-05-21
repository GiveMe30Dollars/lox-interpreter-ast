#include "scanner.h"

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

std::string tokenTypeToString(const TokenType type){
    // helper function to cast TokenType enum to string
    switch (type){
        case LEFT_PAREN: return "LEFT_PAREN";
        case RIGHT_PAREN: return "RIGHT_PAREN";
        case LEFT_BRACE: return "LEFT_BRACE";
        case RIGHT_BRACE: return "RIGHT_BRACE";

        case COMMA: return "COMMA";
        case DOT: return "DOT";
        case MINUS: return "MINUS";
        case PLUS: return "PLUS";
        case SEMICOLON: return "SEMICOLON";
        case SLASH: return "SLASH";
        case STAR: return "STAR";

        case BANG: return "BANG";
        case BANG_EQUAL: return "BANG_EQUAL";
        case EQUAL: return "EQUAL";
        case EQUAL_EQUAL: return "EQUAL_EQUAL";
        case GREATER: return "GREATER";
        case GREATER_EQUAL: return "GREATER_EQUAL";
        case LESS: return "LESS";
        case LESS_EQUAL: return "LESS_EQUAL";

        case IDENTIFIER: return "IDENTIFIER";
        case STRING: return "STRING";
        case NUMBER: return "NUMBER";

        case _EOF: return "EOF";

        default:
            std::cerr << "Unhandled tokenTypeToString call for TokenType!";
            return "";
    }
}


bool isDigit(const char c){
    return (c >= '0' && c <= '9');
}
bool isLetter (const char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

class Token {
    // data structure for classifying and storing lexemes during scanning
    private:
        TokenType type;
        std::string lexeme;
        std::string literal;
        int line;

    public:
        Token(TokenType type, std::string lexeme, std::string literal) {
            this->type = type;
            this->lexeme = lexeme;
            this->literal = literal;
        }
        Token(TokenType type, std::string lexeme) : Token(type, lexeme, "null") {
            if (type == TokenType::IDENTIFIER || type == TokenType::STRING || type == TokenType::NUMBER){
                std::cerr << "No literal value for token declaration of type " << tokenTypeToString(type) << " !";
            }
        }

        std::string toString() {
            return tokenTypeToString(type) + " " + lexeme + " " + literal;
        }
};

class Scanner{
    private:
        std::string source;
        int start;
        int curr;
        int line;

        bool isAtEnd(){
            // returns true if at end of source
            return curr >= source.length();
        }
        char advance(){
            // consumes and returns the upcoming character
            return source[curr++];
        }
        char peek(){
            // returns the upcoming character without consuming it
            return source[curr];
        }
        char peekNext(){
            // returns the second upcoming character without consuming it
            return source[curr+1];
        }
        bool match(char c){
            // checks if upcoming character matches c. If yes, consume the character and return true
            if (isAtEnd() || peek() != c) return false;
            else {
                advance();
                return true;
            }
        }
        void addToken(TokenType type){
            // add token based on pointers
            std::string lexeme = source.substr(start, curr - start);
            
            if (type = STRING) tokens.push_back(new Token(type, lexeme, lexeme));
            else tokens.push_back(new Token(type, lexeme));
        }

    public:
        bool hasError;
        std::vector<Token*> tokens;

        Scanner(std::string source){
            // initializes scanner object
            // immediately scans the source text and generates corresponding tokens in the order they appear
            this->source = source;
            this->tokens = {};
            start = 0;
            curr = 0;
            line = 1;
            hasError = false;
            scan();
        }

    private:
        void scan(){

            // execute until end of source
            while (!isAtEnd()){

                // switch on current character
                char c = advance();
                switch (c){

                    // single-letter symbols (except slash)
                    case '(': addToken(LEFT_PAREN); break;
                    case ')': addToken(RIGHT_PAREN); break;
                    case '{': addToken(LEFT_BRACE); break;
                    case '}': addToken(RIGHT_BRACE); break;
                    case ',': addToken(COMMA); break;
                    case '.': addToken(DOT); break;
                    case '-': addToken(MINUS); break;
                    case '+': addToken(PLUS); break;
                    case ';': addToken(SEMICOLON); break;
                    case '*': addToken(STAR); break;

                    // single or double-letter symbols
                    case '=':
                        addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
                    case '!':
                        addToken(match('=') ? BANG_EQUAL : BANG); break;
                    case '>':
                        addToken(match('=') ? GREATER_EQUAL : GREATER); break;
                    case '<':
                        addToken(match('=') ? LESS_EQUAL : LESS); break;

                    // slash or comment
                    // if is comment, escape all characters up to \n
                    case '/':
                        if (match('/')){
                            // if is comment, escape all characters until linebreak
                            // (do not consume \n character)
                            while (!isAtEnd() && peek() != '\n') advance();
                        } else {
                            addToken(SLASH);
                        }
                        break;

                    // string literals
                    // read until closing double quotation mark
                    // throw error if it doesn't exist
                    case '"': 
                        while (!isAtEnd()){
                            if (advance() == '"'){
                                addToken(STRING);
                                break;
                            }
                        }
                        std::cerr << "[line " << line << "] Error: Unterminated string.";
                        hasError = true;
                        break;

                    // blankspace and linebreaks
                    case ' ': break;
                    case '\t': break;
                    case '\n':
                        line++; break;

                    // unhandled characters
                    default:
                        std::cerr << "[line " << line << "] Error: Unexpected character: " << c << "\n";
                        break;
                }
                
                start = curr;
            }

            // add end-of-file token
            addToken(_EOF);
        }
};