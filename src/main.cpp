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
    std::cerr << "Usage: ./lox.sh tokenize <filename>" << std::endl;
    std::cerr << "Usage: ./lox.sh parse <filename>" << std::endl;
    std::cerr << "Usage: ./lox.sh evaluate <filename>" << std::endl;
    std::cerr << "Usage: ./lox.sh run <filename>" << std::endl;
    std::cerr << "Usage: ./lox.sh repl" << std::endl;
    std::cerr << "Usage: ./lox.sh" << std::endl;
    return 1;
}

int main(int argc, char *argv[]) {
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc > 3) {
        return usageInfo();
    }
    std::string command;
    if (argc == 1) command = "repl";
    else {
        command = argv[1];
        if (argc < 3 && command != "repl")
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
        std::cout << " \n-----LOX REPL-----\nTo exit, type 'exit'.\n";
        while(true){
            std::string replInput = "";
            std::getline(std::cin, replInput);
            if (replInput == "exit") break;
            Lox::run(replInput, true);
            if (Lox::hasCompileError) std::cout << "returned code: 65\n";
            if (Lox::hasRuntimeError) std::cout << "returned code: 70\n";
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


