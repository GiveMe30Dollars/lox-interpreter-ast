#include "ASTPrinter.hpp"

std::string ASTPrinter::print(std::shared_ptr<Expr> expr){
    return std::any_cast<std::string>(expr);
}

std::any ASTPrinter::visit(std::shared_ptr<Expr> expr){
    return expr->accept(*this);
}

std::any ASTPrinter::visitLiteral(std::shared_ptr<Literal> curr){
    std::cerr << "called literal\n";
    return curr->obj.toString(true);
}

std::any ASTPrinter::visitGrouping(std::shared_ptr<Grouping> curr){
    return "(group " + print(curr->expr) + ")";
}

std::any ASTPrinter::visitUnary(std::shared_ptr<Unary> curr){
    return "(" + curr->op.lexeme + " " + print(curr->expr) + ")";
}

std::any ASTPrinter::visitBinary(std::shared_ptr<Binary> curr){
    std::cerr << "called binary\n";
    return "(" + curr->op.lexeme + " " + print(curr->left) + " " + print(curr->right) + ")";
}

