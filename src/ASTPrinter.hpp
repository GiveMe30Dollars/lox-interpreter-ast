
#include "expr.hpp"

class ASTPrinter : Visitor{
    public:
    // prints a constructed expression via the Visitor design pattern.
    std::string print(std::shared_ptr<Expr> expr);
    std::any visitLiteral(std::shared_ptr<Literal> expr);
    std::any visitGrouping(std::shared_ptr<Grouping> expr);
    std::any visitUnary(std::shared_ptr<Unary> expr);
    std::any visitBinary(std::shared_ptr<Binary> expr);
};