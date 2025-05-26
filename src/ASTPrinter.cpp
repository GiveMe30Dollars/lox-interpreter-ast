#include "ASTPrinter.hpp"

std::string ASTPrinter::print(std::shared_ptr<Expr> expr){
    return std::any_cast<std::string>(visit(expr));
}

std::any ASTPrinter::visit(std::shared_ptr<Expr> expr){
    return expr->accept(*this);
}

std::string ASTPrinter::print(std::shared_ptr<Stmt> stmt){
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
    return "var:" + curr->name.lexeme;
}
std::any ASTPrinter::visitAssign(std::shared_ptr<Assign> curr){
    return "(assign var:" + curr->name.lexeme + " " + print(curr->expr) + ")";
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