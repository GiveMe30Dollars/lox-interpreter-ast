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
std::any ASTPrinter::visitLiteralExpr(std::shared_ptr<LiteralExpr> curr){
    std::cerr << "at literal!\n";
    return curr->obj.toString(true);
}
std::any ASTPrinter::visitGroupingExpr(std::shared_ptr<GroupingExpr> curr){
    return "(group " + print(curr->expr) + ")";
}
std::any ASTPrinter::visitUnaryExpr(std::shared_ptr<UnaryExpr> curr){
    return "(" + curr->op.lexeme + " " + print(curr->expr) + ")";
}
std::any ASTPrinter::visitBinaryExpr(std::shared_ptr<BinaryExpr> curr){
    return "(" + curr->op.lexeme + " " + print(curr->left) + " " + print(curr->right) + ")";
}

std::any ASTPrinter::visitVariableExpr(std::shared_ptr<VariableExpr> curr){
    return curr->name.lexeme;
}
std::any ASTPrinter::visitAssignExpr(std::shared_ptr<AssignExpr> curr){
    return "(assign " + curr->name.lexeme + " " + print(curr->expr) + ")";
}
std::any ASTPrinter::visitLogicalExpr(std::shared_ptr<LogicalExpr> curr){
    return "(" + curr->op.lexeme + " " + print(curr->left) + " " + print(curr->right) + ")";
}

std::any ASTPrinter::visitCallExpr(std::shared_ptr<CallExpr> curr){
    return "(call " + print(curr->callee) + ")";
}
std::any ASTPrinter::visitGetExpr(std::shared_ptr<GetExpr> curr){
    return "(get " + print(curr->expr) + "." + curr->name.lexeme + ")";
}
std::any ASTPrinter::visitSetExpr(std::shared_ptr<SetExpr> curr){
    return "(set " + print(curr->expr) + "." + curr->name.lexeme + " -> " + print(curr->value) + ")";
}
std::any ASTPrinter::visitThisExpr(std::shared_ptr<ThisExpr> curr){
    // yes, this cast is necessary. string literals are read as const char[]
    return std::string("this");
}

// ---STATEMENTS---
std::any ASTPrinter::visitExpressionStmt(std::shared_ptr<ExpressionStmt> curr){
    return "(expr " + print(curr->expr) +")";
}
std::any ASTPrinter::visitPrintStmt(std::shared_ptr<PrintStmt> curr){
    return "(print " + print(curr->expr) +")";
}
std::any ASTPrinter::visitVarStmt(std::shared_ptr<VarStmt> curr){
    return "(varDecl: " + curr->name.lexeme + " " + (curr->initializer == nullptr ? "nil" : print(curr->initializer)) + ")";
}
std::any ASTPrinter::visitBlockStmt(std::shared_ptr<BlockStmt> curr){
    std::string s = "";
    currIndent += increment;
    for (std::shared_ptr<Stmt> stmt : curr->statements){
        s = s + std::string(currIndent, ' ') + print(stmt) + "\n";
    }
    currIndent -= increment;
    return "(block:\n" + s + std::string(currIndent, ' ') + "end)";
}

// ---STMT (CONTROL FLOW)---
std::any ASTPrinter::visitIfStmt(std::shared_ptr<IfStmt> curr){
    return "(if " + print(curr->condition) 
        + " then " + print(curr->thenBranch) 
        + (curr->elseBranch ? " else "  + print(curr->elseBranch): "") + ")";
}
std::any ASTPrinter::visitWhileStmt(std::shared_ptr<WhileStmt> curr){
    return "(while " + print(curr->condition)
        + " " + print(curr->body) + ")";
}

// ---STMT (FUNCTIONS AND CLASSES)---
std::any ASTPrinter::visitFunctionStmt(std::shared_ptr<FunctionStmt> curr){
    std::string s = "";
    currIndent += increment;
    for (std::shared_ptr<Stmt> stmt : curr->body){
        s = s + std::string(currIndent, ' ') + print(stmt) + "\n";
    }
    currIndent -= increment;

    std::string args = "";
    for (Token token : curr->params)
        args = args + " " + token.lexeme;
    if (args == "") args = " none";

    std::string output = "(funDecl: " + curr->name.lexeme + " args" + args + "\n" 
        + s + std::string(currIndent, ' ') + "end)";
    
    return output;
}
std::any ASTPrinter::visitReturnStmt(std::shared_ptr<ReturnStmt> curr){
    return "(return " + print(curr->expr) + ")";
}
std::any ASTPrinter::visitClassStmt(std::shared_ptr<ClassStmt> curr){
    std::string s = "";
    currIndent += increment;
    for (std::shared_ptr<FunctionStmt> func : curr->methods){
        s = s + std::string(currIndent, ' ') + print(func) + "\n";
    }
    currIndent -= increment;

    std::string output = "(classDecl: " + curr->name.lexeme + "\n" 
        + s + std::string(currIndent, ' ') + "end)";

    return output;
}
