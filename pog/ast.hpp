#pragma once
#include <memory>
#include <string>
#include <vector>
#include "token.hpp"

namespace pog {

    // Forward declarations
    class Expression;
    class Statement;

    // Base class for all expressions
    class Expression {
    public:
        virtual ~Expression() = default;
    };

    // Literal expression (numbers, strings)
    class Literal : public Expression {
    public:
        explicit Literal(std::string value) : value(std::move(value)) {}
        std::string value;
    };

    // Binary expression (e.g., a + b, x > 10)
    class Binary : public Expression {
    public:
        Binary(std::unique_ptr<Expression> left, Token op, std::unique_ptr<Expression> right)
            : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

        std::unique_ptr<Expression> left;
        Token op;
        std::unique_ptr<Expression> right;
    };

    // Base class for all statements
    class Statement {
    public:
        virtual ~Statement() = default;
    };

    // Expression statement (e.g., poggers("Hello");)
    class ExpressionStmt : public Statement {
    public:
        explicit ExpressionStmt(std::unique_ptr<Expression> expression)
            : expression(std::move(expression)) {}

        std::unique_ptr<Expression> expression;
    };

    // In ast.hpp, add:
    class IfStatement : public Statement {
    public:
        IfStatement(std::unique_ptr<Expression> condition,
            std::unique_ptr<Statement> thenBranch,
            std::unique_ptr<Statement> elseBranch)
            : condition(std::move(condition))
            , thenBranch(std::move(thenBranch))
            , elseBranch(std::move(elseBranch)) {}

        std::unique_ptr<Expression> condition;
        std::unique_ptr<Statement> thenBranch;
        std::unique_ptr<Statement> elseBranch;
    };

    // Unary expression (e.g., !true, -5)
    class Unary : public Expression {
    public:
        Unary(Token op, std::unique_ptr<Expression> right)
            : op(std::move(op))
            , right(std::move(right)) {}

        Token op;
        std::unique_ptr<Expression> right;
    };

    // Variable expression (identifiers like x, y, etc.)
    class Variable : public Expression {
    public:
        explicit Variable(Token name)
            : name(std::move(name)) {}

        Token name;
    };

    // Grouping expression (parenthesized expressions)
    class Grouping : public Expression {
    public:
        explicit Grouping(std::unique_ptr<Expression> expression)
            : expression(std::move(expression)) {}

        std::unique_ptr<Expression> expression;
    };

    class FunctionStmt : public Statement {
    public:
        FunctionStmt(Token name,
            std::vector<Token> params,
            std::vector<std::unique_ptr<Statement>> body)
            : name(std::move(name))
            , parameters(std::move(params))
            , body(std::move(body)) {}

        Token name;
        std::vector<Token> parameters;
        std::vector<std::unique_ptr<Statement>> body;
    };

    class CallExpression : public Expression {
    public:
        CallExpression(std::unique_ptr<Expression> callee,
            Token paren,
            std::vector<std::unique_ptr<Expression>> arguments)
            : callee(std::move(callee))
            , paren(std::move(paren))
            , arguments(std::move(arguments)) {}

        std::unique_ptr<Expression> callee;
        Token paren;
        std::vector<std::unique_ptr<Expression>> arguments;
    };

    class BlockStmt : public Statement {
    public:
        explicit BlockStmt(std::vector<std::unique_ptr<Statement>> statements)
            : statements(std::move(statements)) {}

        std::vector<std::unique_ptr<Statement>> statements;
    };

    class WhileStmt : public Statement {
    public:
        WhileStmt(std::unique_ptr<Expression> condition,
            std::unique_ptr<Statement> body)
            : condition(std::move(condition))
            , body(std::move(body)) {}

        std::unique_ptr<Expression> condition;
        std::unique_ptr<Statement> body;
    };

    class AssignExpr : public Expression {
    public:
        AssignExpr(Token name, std::unique_ptr<Expression> value)
            : name(std::move(name))
            , value(std::move(value)) {}

        Token name;
        std::unique_ptr<Expression> value;
    };

} // namespace pog