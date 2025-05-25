#include "expr.hpp"
#include "loxerror.hpp"

class Interpreter : ExprVisitor{
    // Interprets an AST and retuns an object
    // via the Visitor design pattern.
    public:
    Object interpret(std::shared_ptr<Expr> curr);
    std::any visit(std::shared_ptr<Expr> curr) override;
    std::any visitLiteral(std::shared_ptr<Literal> curr) override;
    std::any visitGrouping(std::shared_ptr<Grouping> curr) override;
    std::any visitUnary(std::shared_ptr<Unary> curr) override;
    std::any visitBinary(std::shared_ptr<Binary> curr) override;
    
    std::any visitVariable(std::shared_ptr<Variable> curr) override;
    std::any visitAssign(std::shared_ptr<Assign> curr) override;

    private:
    bool isTruthy(Object obj);
    bool isEqual(Object a, Object b);
    LoxError::RuntimeError error(Token op, std::string message);
};