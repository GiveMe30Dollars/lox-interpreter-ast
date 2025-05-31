#include "lox.hpp"
// not required, but useful for debugging
// #include "ASTPrinter.hpp"

bool Lox::hasCompileError = false;
bool Lox::hasRuntimeError = false;
Interpreter Lox::interpreter;

void Lox::run(std::string source, bool parseExpr){
    Lox::hasCompileError = false;
    Lox::hasRuntimeError = false;

    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan();
    if (scanner.hasError){
        hasCompileError = true;
        return;
    }

    StmtParser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse(parseExpr);
    if (parser.hasError){
        hasCompileError = true;
        return;
    }

    // ASTPrinter printer;
    // for (auto stmt : statements) std::cerr << printer.print(stmt) << "\n";
    
    Resolver resolver(interpreter);
    resolver.resolve(statements);
    if (resolver.hasError){
        hasCompileError = true;
        return;
    }

    try{
        interpreter.execute(statements);
    }
    catch (LoxError::RuntimeError err){
        err.print();
        Lox::hasRuntimeError = true;
        return;
    }
}

static inline void replInfo(void){
    // Helper function for Lox REPL session.
    std::cout << "To exit, type 'exit'. For multiline input, toggle 'multiline'.\n";
    std::cout << "For more information on Lox, visit: \nhttps://craftinginterpreters.com/the-lox-language.html\n";
}

void Lox::repl(){
    std::cout << " \n----- LOX REPL -----\n";
    replInfo();

    bool multiline = false;

    while(true){

        // get input as string. multiline input supported
        std::string replInput = "";
        if (multiline){
            replInput = "";
            std::string s = "";
            int line = 1;
            while (true){
                std::printf("%02d| ", line);
                std::getline(std::cin, s);
                if (s == ""){
                    break;
                }
                else {
                    // add linebreak only if not first line
                    replInput = replInput + (line == 1 ? "" : "\n") + s;
                    line++;
                }
            }
        }
        else {
            std::cout << ">>> ";
            std::getline(std::cin, replInput);
        }
        
        // Special handling for REPL keywords
        if (replInput == "exit"){
            std::cout << "Lox REPL session ended.\n";
            return;
        }
        if (replInput == "multiline"){
            multiline = !multiline;
            std::cout << "Multiline input " << (multiline ? "enabled" : "disabled") << ".\n";
            continue;
        }
        if (replInput == ""){
            std::cout << "Empty input. Need help?\n";
            replInfo();
            continue;
        }

        // run input string.
        Lox::run(replInput, true);
        if (Lox::hasCompileError) std::cout << "[return code] 65\n";
        if (Lox::hasRuntimeError) std::cout << "[return code] 70\n";
    }
}