#include "scanner.hpp"

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

std::unordered_map<std::string, Token::TokenType> Scanner::reservedKeywords = {
    {"and", Token::AND},
    {"class", Token::CLASS},
    {"else", Token::ELSE},
    {"false", Token::FALSE},
    {"fun", Token::FUN},
    {"for", Token::FOR},
    {"if", Token::IF},
    {"nil", Token::NIL},
    {"or", Token::OR},
    {"print", Token::PRINT},
    {"return", Token::RETURN},
    {"super", Token::SUPER},
    {"this", Token::THIS},
    {"true", Token::TRUE},
    {"var", Token::VAR},
    {"while", Token::WHILE},
};


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
void Scanner::addToken(Token::TokenType type){
    // add token based on pointers
    // special handling for string literals, numbers and identifiers

    std::string lexeme = source.substr(start, curr - start);

    if (type == Token::STRING){
        std::string literal = lexeme.substr(1, lexeme.length() - 2);
        tokens.push_back(Token(type, lexeme, Object::string(literal), line));
    }
    else if (type == Token::NUMBER){
        double val = stod(lexeme);
        tokens.push_back(Token(type, lexeme, Object::number(val), line));
    }
    else if (type == Token::IDENTIFIER){
        // check if identifier is a reserved keyword
        // if so, add token as reserved TokenType
        if (reservedKeywords.count(lexeme)){
            type = reservedKeywords.at(lexeme);
            tokens.push_back(Token(type, lexeme, Object::nil(), line));
        } else {
            tokens.push_back(Token(type, lexeme, Object::nil(), line));
        }
    }
    else tokens.push_back(Token(type, lexeme, Object::nil(), line));
}
void Scanner::error(int line, std::string message){
    hasError = true;
    LoxError::ScanningError(line, message).print();
}

Scanner::Scanner(std::string source){
    // initializes scanner object
    this->source = source;
}

std::vector<Token> Scanner::scan(){
    // scans the source text and generates corresponding tokens in the order they appear
    this->tokens = {};
    start = 0;
    curr = 0;
    line = 1;
    hasError = false;

    // execute until end of source
    while (!isAtEnd()){

        // switch on current character
        char c = advance();
        switch (c){

            // single-letter symbols (except slash)
            case '(': addToken(Token::LEFT_PAREN); break;
            case ')': addToken(Token::RIGHT_PAREN); break;
            case '{': addToken(Token::LEFT_BRACE); break;
            case '}': addToken(Token::RIGHT_BRACE); break;
            case ',': addToken(Token::COMMA); break;
            case '.': addToken(Token::DOT); break;
            case '-': addToken(Token::MINUS); break;
            case '+': addToken(Token::PLUS); break;
            case ';': addToken(Token::SEMICOLON); break;
            case '*': addToken(Token::STAR); break;

            // single or double-letter symbols
            case '=':
                addToken(match('=') ? Token::EQUAL_EQUAL : Token::EQUAL); break;
            case '!':
                addToken(match('=') ? Token::BANG_EQUAL : Token::BANG); break;
            case '>':
                addToken(match('=') ? Token::GREATER_EQUAL : Token::GREATER); break;
            case '<':
                addToken(match('=') ? Token::LESS_EQUAL : Token::LESS); break;

            // slash or comment
            // if is comment, escape all characters up to \n
            case '/':
                if (match('/')){
                    // if is comment, escape all characters until linebreak
                    // (do not consume \n character)
                    while (!isAtEnd() && peek() != '\n') advance();
                } else {
                    addToken(Token::SLASH);
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
                }
                else if (isAlpha(c)){
                    scanIdentifier();
                } 
                else {
                    std::string unhandledErr = "Unexpected character: " + std::string(1,c);
                    error(line, unhandledErr);
                }
                break;
        }
        
        // move start from token created
        start = curr;
    }
    
    // add end-of-file token
    addToken(Token::_EOF);
    return this->tokens;
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
    
    addToken(Token::STRING);
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

    addToken(Token::NUMBER);
}

void Scanner::scanIdentifier(){
    // reads identifiers and reserved keywords
    // read until all alphanumeric characters consumed
    while (isAlphaNumeric(peek())) advance();
    addToken(Token::IDENTIFIER);
}
