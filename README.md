[![progress-banner](https://backend.codecrafters.io/progress/interpreter/0877c070-7800-493c-8aa4-f0dbca4625bb)](https://app.codecrafters.io/users/codecrafters-bot?r=2qF)

## AST-Based Lox Interpreter In C++

This is a completed C++ implemetation of the Lox interpreter as described in the first half of the book:  
- **"Crafting Interpreters"**: https://craftinginterpreters.com/ by Robert Nystrom.  

This implemetation is initialized, initially hosted on and tested on Codecrafters.io:  
- **"Build your own Interpreter"** Challenge: https://app.codecrafters.io/courses/interpreter/overview.  

This repository was first initialized on 21 May 2025, and has fully implemented Lox and cleared all Codecrafters.io testcases by 30 May 2025. 
Subsequent additions to the code may be done to add new features to the implemetation.

## USAGE

To fulfill the testing requirements as described in Codecrafters.io, the code may be run from the command line in the following ways:  

- `./your_program.sh tokenize test.lox`: Scans the string as stored in `test.lox`, then prints each resultant token on `std::cout`  
- `./your_program.sh parse test.lox`: Scans and parses the string as stored in 'test.lox', then prints the resultant Abstract Syntax Tree as nested expressions in `std::cout`
- `./your_program.sh evaluate test.lox`: Scans, parses and evaluates an expression as stored in `test.lox`, then prints out the value of the evaluation.
- `./your_program.sh run test.lox`: Executes a Lox program as stored in `test.lox`. The file is scanned, parsed, resolved for closures and method binding, then executed line-by-line.

Additionally, the following mode has been added:

- `./your_program.sh repl` / `./your_program.sh`: Opens REPL mode. User input is read and executed on a per-line basis. This is also the default behaviour for directly opening the compiled executable.

To compile and run the project, the following dependencies are required:
- **Any C++ Compiler**
- **CMake**: https://cmake.org/
- **vcpkg**: https://vcpkg.io/en/
- **Any command-line interface that executes `.sh` files.**
  - For Windows and macOS: **Git Bash**: https://git-scm.com/downloads
  - For Linux: *None. Natively supported.*