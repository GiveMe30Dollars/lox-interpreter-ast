#include "expr.hpp"

class Interpreter : Visitor{
    // Interprets an AST and retuns an object
    // via the Visitor design pattern.
    Object interpret(std::shared_ptr<Expr> expr);
    std::any visit(std::shared_ptr<Expr> expr) override;
    std::any visitLiteral(std::shared_ptr<Literal> expr) override;
    std::any visitGrouping(std::shared_ptr<Grouping> expr) override;
    std::any visitUnary(std::shared_ptr<Unary> expr) override;
    std::any visitBinary(std::shared_ptr<Binary> expr) override;
};