#include "expr.hpp"
#include "loxerror.hpp"

class Interpreter : ExprVisitor{
    // Interprets an AST and retuns an object
    // via the Visitor design pattern.
    public:
    Object interpret(std::shared_ptr<Expr> expr);
    std::any visit(std::shared_ptr<Expr> expr) override;
    std::any visitLiteral(std::shared_ptr<Literal> expr) override;
    std::any visitGrouping(std::shared_ptr<Grouping> expr) override;
    std::any visitUnary(std::shared_ptr<Unary> expr) override;
    std::any visitBinary(std::shared_ptr<Binary> expr) override;

    private:
    bool isTruthy(Object obj);
    bool isEqual(Object a, Object b);
    LoxError::RuntimeError error(Token op, std::string message);
};