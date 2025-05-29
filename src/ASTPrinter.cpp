#include "ASTPrinter.hpp"

std::string ASTPrinter::print(std::shared_ptr<Expr> expr){
    if (expr == nullptr) return "expr:null";
    return std::any_cast<std::string>(visit(expr));
}

std::any ASTPrinter::visit(std::shared_ptr<Expr> expr){
    return expr->accept(*this);
}

std::string ASTPrinter::print(std::shared_ptr<Stmt> stmt){
    if (stmt == nullptr) return "stmt:null";
    return std::any_cast<std::string>(visit(stmt));
}
std::any ASTPrinter::visit(std::shared_ptr<Stmt> stmt){
    return stmt->accept(*this);
}

// ---EXPRESSIONS---
std::any ASTPrinter::visitLiteral(std::shared_ptr<Literal> curr){
    return curr->obj.toString(true);
}
std::any ASTPrinter::visitGrouping(std::shared_ptr<Grouping> curr){
    return "(group " + print(curr->expr) + ")";
}
std::any ASTPrinter::visitUnary(std::shared_ptr<Unary> curr){
    return "(" + curr->op.lexeme + " " + print(curr->expr) + ")";
}
std::any ASTPrinter::visitBinary(std::shared_ptr<Binary> curr){
    return "(" + curr->op.lexeme + " " + print(curr->left) + " " + print(curr->right) + ")";
}

std::any ASTPrinter::visitVariable(std::shared_ptr<Variable> curr){
    return curr->name.lexeme;
}
std::any ASTPrinter::visitAssign(std::shared_ptr<Assign> curr){
    return "(assign " + curr->name.lexeme + " " + print(curr->expr) + ")";
}
std::any ASTPrinter::visitLogical(std::shared_ptr<Logical> curr){
    return "(" + curr->op.lexeme + " " + print(curr->left) + " " + print(curr->right) + ")";
}

std::any ASTPrinter::visitCall(std::shared_ptr<Call> curr){
    return "(call " + print(curr->callee) + ")";
}
std::any ASTPrinter::visitGet(std::shared_ptr<Get> curr){
    return "(get " + print(curr->expr) + "." + curr->name.lexeme + ")";
}
std::any ASTPrinter::visitSet(std::shared_ptr<Set> curr){
    return "(set " + print(curr->expr) + "." + curr->name.lexeme + " -> " + print(curr->value) + ")";
}
std::any ASTPrinter::visitThis(std::shared_ptr<This> curr){
    return "this";
}

// ---STATEMENTS---
std::any ASTPrinter::visitExpression(std::shared_ptr<Expression> curr){
    return "(expr " + print(curr->expr) +")";
}
std::any ASTPrinter::visitPrint(std::shared_ptr<Print> curr){
    return "(print " + print(curr->expr) +")";
}
std::any ASTPrinter::visitVar(std::shared_ptr<Var> curr){
    return "(varDecl: " + curr->name.lexeme + " " + (curr->initializer == nullptr ? "nil" : print(curr->initializer)) + ")";
}
std::any ASTPrinter::visitBlock(std::shared_ptr<Block> curr){
    std::string s = "";
    currIndent += increment;
    for (std::shared_ptr<Stmt> stmt : curr->statements){
        s = s + std::string(currIndent, ' ') + print(stmt) + "\n";
    }
    currIndent -= increment;
    return "(block:\n" + s + std::string(currIndent, ' ') + "end)";
}

// ---STMT (CONTROL FLOW)---
std::any ASTPrinter::visitIf(std::shared_ptr<If> curr){
    return "(if " + print(curr->condition) 
        + " then " + print(curr->thenBranch) 
        + (curr->elseBranch ? " else "  + print(curr->elseBranch): "") + ")";
}
std::any ASTPrinter::visitWhile(std::shared_ptr<While> curr){
    return "(while " + print(curr->condition)
        + " " + print(curr->body) + ")";
}

// ---STMT (FUNCTIONS AND CLASSES)---
std::any ASTPrinter::visitFunction(std::shared_ptr<Function> curr){
    std::cerr << "in visitFunction!\n";
    std::string s = "";
    currIndent += increment;
    for (std::shared_ptr<Stmt> stmt : curr->body){
        s = s + "    " + print(stmt) + "\n";
    }
    currIndent -= increment;
    std::string args = "";
    for (Token token : curr->params)
        args = args + token.lexeme + " ";

    std::string output = "(funDecl: " + curr->name.lexeme + " args " + args + "\n" 
        + s + "    " + "end)";
    std::cerr << output << "\n";
    return output;
}
std::any ASTPrinter::visitReturn(std::shared_ptr<Return> curr){
    return "(return " + print(curr->expr) + ")";
}
std::any ASTPrinter::visitClass(std::shared_ptr<Class> curr){
    std::cerr << "in visitClass!\n";
    std::string s = "";
    currIndent += increment;
    for (std::shared_ptr<Function> func : curr->methods){
        s = s + std::string(currIndent, ' ') + print(func) + "\n";
    }
    currIndent -= increment;

    std::string output = "(classDecl: " + curr->name.lexeme + "\n" 
        + s + std::string(currIndent, ' ') + "end)";
    std::cerr << output << "\n";
    return output;
}
