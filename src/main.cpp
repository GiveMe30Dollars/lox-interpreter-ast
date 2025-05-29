#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <unordered_set>
#include <unordered_map>

// main Lox class for scanning, parsing and executing Lox files
#include "lox.hpp"

// components of Lox for testcases
#include "scanner.hpp"
#include "stmtParser.hpp"
#include "ASTPrinter.hpp"

std::string read_file_contents(const std::string& filename);

int usageInfo(){
    std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
    std::cerr << "Usage: ./your_program parse <filename>" << std::endl;
    std::cerr << "Usage: ./your_program evaluate <filename>" << std::endl;
    std::cerr << "Usage: ./your_program run <filename>" << std::endl;
    std::cerr << "Usage: ./your_program run <filename>" << std::endl;
    return 1;
}

int main(int argc, char *argv[]) {
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    // std::cerr << "Logs from your program will appear here!" << std::endl;

    if (argc < 2 || argc > 3) {
        return usageInfo();
    }
    const std::string command = argv[1];
    if (argc < 3 && command != "repl"){
        return usageInfo();
    }

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);
          
        Scanner scanner(file_contents);
        std::vector<Token> tokens = scanner.scan();
        for (Token t : tokens) std::cout << t.toString() << "\n";
        return scanner.hasError ? 65 : 0;
    } 

    if (command == "parse"){
        std::string file_contents = read_file_contents(argv[2]);
          
        Scanner scanner(file_contents);
        std::vector<Token> tokens = scanner.scan();
        if (scanner.hasError) return 65;

        ExprParser parser(tokens);
        std::shared_ptr<Expr> expr = parser.parse();
        if (parser.hasError) return 65;

        ASTPrinter printer;
        std::cout << printer.print(expr) << "\n";

        return 0;
    }

    if (command == "evaluate"){
        std::string file_contents = read_file_contents(argv[2]);
        Lox::run(file_contents, true);
        if (Lox::hasCompileError) return 65;
        if (Lox::hasRuntimeError) return 70;
        return 0;
    }
    if (command == "run"){
        std::string file_contents = read_file_contents(argv[2]);
        Lox::run(file_contents);
        if (Lox::hasCompileError) return 65;
        if (Lox::hasRuntimeError) return 70;
        return 0;
    }
    if (command == "repl"){
        std::cout << " ---LOX REPL---\nTo exit, type exit().\n";
        while(true){
            std::string replInput = "";
            std::cin >> replInput;
            if (replInput == "exit()") break;
            Lox::run(replInput, true);
        }
        return 0;
    }
    
    else {
        std::cerr << "Unknown command: " << command << std::endl;
        return usageInfo();
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


