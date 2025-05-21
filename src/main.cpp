#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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
    private:
        TokenType type;
        std::string lexeme;
        std::string literal;

    public:
        Token(TokenType type, std::string lexeme, std::string literal) {
            this->type = type;
            this->lexeme = lexeme;
            this->literal = literal;
        }
        Token(TokenType type, std::string lexeme) : Token(type, lexeme, "null") {
            if (type == TokenType::IDENTIFIER || type == TokenType::STRING || type == TokenType::NUMBER){
                std::cerr << "Invalid token declaration for " << tokenTypeToString(type) << " !";
            }
        }

        std::string toString() {
            return tokenTypeToString(type) + " " + lexeme + " " + literal + "\n";
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
        
        for (int i = 0; i < file_contents.length(); i++){
            Token* curr;
            switch (file_contents[i]){
                case '(':
                    curr = new Token(LEFT_PAREN, "("); break;
                case ')':
                    curr = new Token(RIGHT_PAREN, ")"); break;
                case '{':
                    curr = new Token(LEFT_BRACE, "{"); break;
                case '}':
                    curr = new Token(RIGHT_BRACE, "}"); break;

                case ',':
                    curr = new Token(COMMA, ","); break;
                case '.':
                    curr = new Token(DOT, "."); break;
                case '-':
                    curr = new Token(MINUS, "-"); break;
                case '+':
                    curr = new Token(PLUS, "+"); break;
                case ';':
                    curr = new Token(SEMICOLON, ";"); break;
                case '*':
                    curr = new Token(STAR, "*"); break;

                default: 
                    std::cerr << "Invalid literal at index " << i << " !";
                    return 1;
            }
            std::cout << curr->toString();
        }
        std::cout << (new Token(_EOF, "", "null"))->toString();
        return 0;
        
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


