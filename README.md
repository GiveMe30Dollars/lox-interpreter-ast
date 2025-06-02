![progress-banner](https://backend.codecrafters.io/progress/interpreter/0877c070-7800-493c-8aa4-f0dbca4625bb)
# AST-Based Lox Interpreter

This is a completed C++ implemetation of the Lox interpreter as described in the first half of the book:  
- **"Crafting Interpreters"**: https://craftinginterpreters.com/ by Robert Nystrom.  

This implemetation is initialized, hosted on and tested on Codecrafters.io:  
- **"Build your own Interpreter"**: https://app.codecrafters.io/courses/interpreter/overview.  

This repository was first initialized on 21 May 2025, and has fully implemented Lox and cleared all Codecrafters.io testcases by 30 May 2025.  
Subsequent additions to the code may be done to add new features to the implemetation.  

As of 1 June 2025, this mirror repository has been disconnected from the original Codecrafters.io repository.

## Usage

To fulfill the testing requirements as described in Codecrafters.io, the code may be run from the command line from this repository in the following ways:  

- `./lox.sh tokenize test.lox`: Scans the string as stored in `test.lox`, then prints each resultant token on `std::cout`.  
- `./lox.sh parse test.lox`: Scans and parses the string as stored in 'test.lox', then prints the resultant Abstract Syntax Tree as nested expressions in `std::cout`.  
- `./lox.sh evaluate test.lox`: Scans, parses and evaluates an expression as stored in `test.lox`, then prints out the value of the evaluation in `std::cout`.
- `./lox.sh run test.lox`: Executes a Lox program as stored in `test.lox`. The file is scanned, parsed, resolved for closures and method binding, then executed line-by-line.  

`test.lox` may be a path to any file, relative to this repository.

Additionally, the following has been added:

- `./lox.sh test.lox`: Identical to `./lox.sh run test.lox`. 
- `./lox.sh`: Opens REPL mode. User input is read and executed on a per-line basis. This is also the default behaviour for directly opening the compiled executable.  
  Two additional commands are available in REPL mode, in addition to all of the Lox syntax:
  - `exit`: Exits the program.
  - `multiline`: Toggles multiline input for the REPL. Multiline input allows for the user to type in an arbitrary number of lines of Lox code, and the code is executed when a blank line is entered.  
    *Note:* Due to constraints in the C++ STL, previous lines cannot be edited for multiline input.

The following return values signify:
- `0`: Operation successfully carried out.
- `1`: Invalid command-line argument.
- `65`: Compilation error in Lox file.
- `70`: Runtime error in Lox file.

## Dependencies

To compile and run the project, the following dependencies are required:
- *Any C++ Compiler.*
- **CMake**: https://cmake.org/
- **vcpkg**: https://vcpkg.io/en/
  - The system environmental variable `VCPKG_ROOT` should be set to the root of the vcpkg directory on your machine.
- *Any command-line interface that executes `.sh` files.*
  - For Windows: **Git Bash**: https://git-scm.com/downloads
  - For Linux and macOS: *None.* Natively supported.  

## Known Issues

- Memory leaks due to circular instance field definitions.  
  As the original AST-based Lox interpreter is implemented in Java, for this implementation, non-literal objects in Lox have been implemented as `std::shared_ptr`, which is encapsulated in the wrapper class `Object` as a one-to-one representation of Lox first-class objects.  
  This leads to instances retaining references to each other and thus never being released by `std::shared_ptr` garbage collection.  
  Consider:
  ```
  class Foo {}

  var i = 0;
  while (i < 1000000) {

    // beginning of scope
    {
      var foo = Foo();
      var bar = Foo();
      foo.bar = bar;
      bar.foo = foo;
    }
    // end of scope
    // reference count of foo and bar == 1;
    // foo and bar are never released by std::shared_ptr garbage collection

    i = i + 1;
  }
  ```  
  This issue will not be addressed in this implementation.



*P.S.* Codecrafters.io tests code on a push-to-run basis, and thus necessitates pushing incomplete and often nonfunctional code onto this repository. This may be reflected in the commit history of this repository.  
The latest version is fully functional and stable.