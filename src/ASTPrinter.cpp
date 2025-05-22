#include "ASTPrinter.hpp"

std::string ASTPrinter::print(std::shared_ptr<Expr> expr){
    return std::any_cast<std::string>(expr->accept(*this));
}

std::any ASTPrinter::visitLiteral(std::shared_ptr<Literal> curr){
    return curr->obj.toString();
}

std::any ASTPrinter::visitGrouping(std::shared_ptr<Grouping> curr){
    return "(group " + print(curr->expr) + ")";
}

std::any ASTPrinter::visitUnary(std::shared_ptr<Unary> curr){
    return "(" + curr->prefix.lexeme + " " + print(curr->expr) + ")";
}

std::any ASTPrinter::visitBinary(std::shared_ptr<Binary> curr){
    return "(" + curr->op.lexeme + " " + print(curr->left) + " " + print(curr->right) + ")";
}

