#include "scanner.hpp"

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


/*class Token {
    private:
        TokenType type;
        std::string lexeme;
        std::string literal;
        int line;
};*/

Token::Token(TokenType type, std::string lexeme, std::string literalString) {
    this->type = type;
    this->lexeme = lexeme;
    this->literalString = literalString;
}
Token::Token(TokenType type, std::string lexeme) : Token(type, lexeme, "null"){}
Token::Token(TokenType type, std::string lexeme, std::string literal, double literalNumber) : Token(type, lexeme, literal){
    this->literalNumber = literalNumber;
}

std::string Token::toString() {
    return tokenTypeToString(type) + " " + lexeme + " " + literalString;
}


/*class Scanner{
    private:
        std::string source;
        int start;
        int curr;
        int line;
    public:
        bool hasError;
        std::vector<Token*> tokens;
};*/

bool Scanner::isAtEnd(){
    // returns true if at end of source
    return curr >= source.length();
}
char Scanner::advance(){
    // consumes and returns the upcoming character
    return source[curr++];
}
char Scanner::peek(){
    // returns the upcoming character without consuming it
    return source[curr];
}
char Scanner::peekNext(){
    // returns the second upcoming character without consuming it
    return source[curr+1];
}
bool Scanner::match(char c){
    // checks if upcoming character matches c. If yes, consume the character and return true
    if (isAtEnd() || peek() != c) return false;
    else {
        advance();
        return true;
    }
}
void Scanner::addToken(TokenType type){
    // add token based on pointers
    std::string lexeme = source.substr(start, curr - start);

    if (type == STRING){
        std::string literal = lexeme.substr(1, lexeme.length() - 2);
        tokens.push_back(new Token(type, lexeme, literal));
    }
    else if (type == NUMBER){
        double val = stod(lexeme);
        std::string literal = lexeme;
        if (floor(val) == val) literal = lexeme + ".0";
        tokens.push_back(new Token(type, lexeme, literal, val));
    }
    else tokens.push_back(new Token(type, lexeme, "null"));
}
void Scanner::error(int line, std::string message){
    std::cerr << "[line " << line << "] Error: " << message << "\n";
    hasError = true;
}

Scanner::Scanner(std::string source){
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

void Scanner::scan(){

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

            // blankspace and linebreaks
            case ' ': break;
            case '\t': break;
            case '\n':
                line++; break;

            // string literal
            case '"': 
                scanStringLiteral(); 
                break;

            // numbers, identifiers, keywords and unhandled characters
            default:
                if (isDigit(c)){
                    scanNumber();
                } else {
                    std::string unhandledErr = "Unexpected character: " + std::string(1,c);
                    error(line, unhandledErr);
                }
                break;
        }
        
        // move start from token created
        start = curr;
    }
    
    // add end-of-file token
    addToken(_EOF);
}

void Scanner::scanStringLiteral(){
    // read until double quotation mark found, then create token
    // throw an error otherwise
    while (peek() != '"' && !isAtEnd()) {
      if (peek() == '\n') line++;
      advance();
    }
    if (isAtEnd()) {
      error(line, "Unterminated string.");
      return;
    }

    // consume the closing "
    advance();
    
    addToken(STRING);
}

void Scanner::scanNumber(){
    // read until curr is not a digit
    while (isDigit(peek())) advance();

    // continue reading if and only if:
    // curr is '.' and the next character is a digit
    // repeat until all consecutive digits consumed
    if (peek() == '.' && isDigit(peekNext())){
        // consume '.'
        advance();
        // consume subsequent consecutive digits
        while (isDigit(peek())) advance();
    }

    addToken(NUMBER);
}
