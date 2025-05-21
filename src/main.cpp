#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

std::string read_file_contents(const std::string& filename);

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

std::string tokenTypeToString(TokenType type){
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
            return start >= source.length();
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
            // checks if upcoming character matches c. If yes, consume the character and return true.
            if (isAtEnd() || peek() != c) return false;
            else {
                advance();
                return true;
            }
        }
        void addToken(TokenType type){
            tokens.push_back(new Token(type, source.substr(start, curr - start)));
        }

    public:
        bool hasError;
        std::vector<Token*> tokens;

        Scanner(std::string source){
            // initializes scanner object. immediately scans the source text and generates corresponding tokens in the order they appear
            this->source = source;
            this->tokens = {};
            scan();
        }

        void scan(){
            start = 0;
            curr = 0;
            line = 1;
            hasError = false;

            while (!isAtEnd()){
                char c = advance();
                switch (c){
                    case '(':
                        addToken(LEFT_PAREN); break;
                    case ')':
                        addToken(RIGHT_PAREN); break;
                    case '{':
                        addToken(LEFT_BRACE); break;
                    case '}':
                        addToken(RIGHT_BRACE); break;

                    case ',':
                        addToken(COMMA); break;
                    case '.':
                        addToken(DOT); break;
                    case '-':
                        addToken(MINUS); break;
                    case '+':
                        addToken(PLUS); break;
                    case ';':
                        addToken(SEMICOLON); break;
                    case '*':
                        addToken(STAR); break;

                    case '=':
                        addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
                    case '!':
                        addToken(match('=') ? BANG_EQUAL : BANG); break;
                    case '>':
                        addToken(match('=') ? GREATER_EQUAL : GREATER); break;
                    case '<':
                        addToken(match('=') ? LESS_EQUAL : LESS); break;

                    case '/':
                        if (match('/')){
                            while (!isAtEnd() && peek() != '\n') advance();
                        } else {
                            addToken(SLASH);
                        }
                        break;
                    case '\n':
                        line++; break;

                    default: 
                        std::cerr << "[line " << line << "] Error: Unexpected character: " << c << "\n";
                        hasError = true;
                        break;
                }
                start = curr;
            }
            addToken(_EOF);
        }
};



int main(int argc, char *argv[]) {
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cerr << "Logs from your program will appear here!" << std::endl;

    if (argc < 3) {
        std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);
          
        Scanner* scanner = new Scanner(file_contents);
        for (Token* t : scanner->tokens) std::cout << t->toString() << "\n";
        return scanner->hasError ? 65 : 0;
        
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }
    return 0;
}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}


