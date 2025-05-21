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

        std::string toString() {
            return type + " " + lexeme + " " + literal;
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
                    curr = new Token(LEFT_PAREN, "(", ""); break;
                case ')':
                    curr = new Token(RIGHT_PAREN, ")", ""); break;
                case '{':
                    curr = new Token(LEFT_BRACE, "{", ""); break;
                case '}':
                    curr = new Token(RIGHT_BRACE, "}", ""); break;
                default: 
                    std::cerr << "Invalid literal at index " << i << " !";
                    return 1;
            }
            curr->toString();
        }
        (new Token(_EOF, "", ""))->toString();
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


