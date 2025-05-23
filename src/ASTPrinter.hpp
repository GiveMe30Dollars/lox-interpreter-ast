
#include "expr.hpp"

class ASTPrinter : Visitor{
    // Prints a constructed expression as a string
    // via the Visitor design pattern.
    public:
        std::string print(std::shared_ptr<Expr> expr);
        std::any visit(std::shared_ptr<Expr> curr) override;
        std::any visitLiteral(std::shared_ptr<Literal> curr) override;
        std::any visitGrouping(std::shared_ptr<Grouping> curr) override;
        std::any visitUnary(std::shared_ptr<Unary> curr) override;
        std::any visitBinary(std::shared_ptr<Binary> curr) override;
};