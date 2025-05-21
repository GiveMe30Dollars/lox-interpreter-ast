#include "scanner.hpp"

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

Token::Token(TokenType type, std::string lexeme, std::string literalString) {
    this->type = type;
    this->lexeme = lexeme;
    this->literalString = literalString;
}
Token::Token(TokenType type, std::string lexeme, std::string literal, double literalNumber) : Token(type, lexeme, literal){
    this->literalNumber = literalNumber;
}

std::string Token::toString() {
    return tokenTypeName.at(type) + " " + lexeme + " " + literalString;
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
    // special handling for string literals, numbers and identifiers

    std::string lexeme = source.substr(start, curr - start);

    if (type == STRING){
        std::string literal = lexeme.substr(1, lexeme.length() - 2);
        tokens.push_back(new Token(type, lexeme, literal));
    }
    else if (type == NUMBER){
        double val = stod(lexeme);
        std::string literal = lexeme;

        // overwrite string literal to add .0 if the number is an integer
        if (floor(val) == val)
            literal = std::to_string((int)floor(val)) + ".0";
        tokens.push_back(new Token(type, lexeme, literal, val));
    }
    else if (type == IDENTIFIER){
        // check if identifier is a reserved keyword
        if (reservedKeywords.count(lexeme)){
            type = reservedKeywords.at(lexeme);
            tokens.push_back(new Token(type, lexeme, "null"));
        } else {
            tokens.push_back(new Token(type, lexeme, "null"));
        }
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

void Scanner::scanIdentifier(){
    // reads identifiers and reserved keywords
    // read until all alphanumeric characters consumed
    while (isAlphaNumeric(peek())) advance();
    addToken(IDENTIFIER);
}
