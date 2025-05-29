#include "ASTPrinter.hpp"

std::string ASTPrinter::print(std::shared_ptr<Expr> expr){
    if (expr == nullptr) return "expr:null";
    ASTPrinter printer;
    return std::any_cast<std::string>(printer.visit(expr));
}

std::any ASTPrinter::visit(std::shared_ptr<Expr> expr){
    return expr->accept(*this);
}

std::string ASTPrinter::print(std::shared_ptr<Stmt> stmt){
    if (stmt == nullptr) return "stmt:null";
    ASTPrinter printer;
    return std::any_cast<std::string>(printer.visit(stmt));
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
    return "var:" + curr->name.lexeme;
}
std::any ASTPrinter::visitAssign(std::shared_ptr<Assign> curr){
    return "(assign var:" + curr->name.lexeme + " " + print(curr->expr) + ")";
}
std::any ASTPrinter::visitLogical(std::shared_ptr<Logical> curr){
    return "(logical:" + curr->op.lexeme + " " + print(curr->left) + " " + print(curr->right) + ")";
}

std::any ASTPrinter::visitCall(std::shared_ptr<Call> curr){
    return "(call:" + print(curr->callee) + ")";
}
std::any ASTPrinter::visitGet(std::shared_ptr<Get> curr){
    return "(get:" + print(curr->expr) + "." + curr->name.lexeme + ")";
}
std::any ASTPrinter::visitSet(std::shared_ptr<Set> curr){
    return "(get:" + print(curr->expr) + "." + curr->name.lexeme + " to " + print(curr->value) + ")";
}

// ---STATEMENTS---
std::any ASTPrinter::visitExpression(std::shared_ptr<Expression> curr){
    return "(expr " + print(curr->expr) +")";
}
std::any ASTPrinter::visitPrint(std::shared_ptr<Print> curr){
    return "(print " + print(curr->expr) +")";
}
std::any ASTPrinter::visitVar(std::shared_ptr<Var> curr){
    return "(declare var:" + curr->name.lexeme + " " + (curr->initializer == nullptr ? "nil" : print(curr->initializer)) + ")";
}
std::any ASTPrinter::visitBlock(std::shared_ptr<Block> curr){
    std::string s = "";
    for (std::shared_ptr<Stmt> stmt : curr->statements){
        s = s + " " + print(stmt);
    }
    return "(block" + s + ")";
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
    return "(funDecl:" + curr->name.toString() + ")";
}
std::any ASTPrinter::visitReturn(std::shared_ptr<Return> curr){
    return "(return " + print(curr->expr) + ")";
}
std::any ASTPrinter::visitClass(std::shared_ptr<Class> curr){
    return "(classDecl: " + curr->name.lexeme + ")";
}
