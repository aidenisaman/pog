#pragma once
#include "ast.hpp"
#include "environment.hpp"
#include <any>
#include <memory>
#include <stdexcept>

namespace pog {

    class RuntimeError : public std::runtime_error {
    public:
        RuntimeError(const Token& token, const std::string& message)
            : std::runtime_error(message), token(token) {}

        Token token;
    };

    class Interpreter {
    public:
        Interpreter();
        void interpret(const std::vector<std::unique_ptr<Statement>>& statements);

    private:
        // Statement visitors
        void executeBlock(const std::vector<std::unique_ptr<Statement>>& statements,
            std::shared_ptr<Environment> environment);
        void visitBlockStmt(const BlockStmt* stmt);
        void visitExpressionStmt(const ExpressionStmt* stmt);
        void visitFunctionStmt(const FunctionStmt* stmt);
        void visitIfStmt(const IfStatement* stmt);
        void visitWhileStmt(const WhileStmt* stmt);

        // Expression visitors
        std::any visitAssignExpr(const AssignExpr* expr);
        std::any visitBinaryExpr(const Binary* expr);
        std::any visitCallExpr(const CallExpression* expr);
        std::any visitGroupingExpr(const Grouping* expr);
        std::any visitLiteralExpr(const Literal* expr);
        std::any visitUnaryExpr(const Unary* expr);
        std::any visitVariableExpr(const Variable* expr);

        // Helper methods
        std::any evaluate(const Expression* expr);
        void execute(const Statement* stmt);
        bool isTruthy(const std::any& object);
        bool isEqual(const std::any& a, const std::any& b);
        void checkNumberOperand(const Token& op, const std::any& operand);
        void checkNumberOperands(const Token& op, const std::any& left, const std::any& right);
        std::string stringify(const std::any& object);

        std::shared_ptr<Environment> environment;
        std::shared_ptr<Environment> globals;
    };

} // namespace pog