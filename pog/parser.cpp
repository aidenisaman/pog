/*******************************************************************************
 * POG Language Interpreter
 * parser.cpp
 *
 * Implementation of the recursive descent parser for the POG language.
 * Converts a sequence of tokens into an Abstract Syntax Tree (AST).
 *
 * The parser implements the following grammar:
 * program     → statement* EOF
 * statement   → exprStmt | ifStmt | functionDecl
 * functionDecl→ "pogchamp" IDENTIFIER "(" parameters? ")" block
 * ifStmt      → "pepega" "(" expression ")" statement ("weirdchamp" statement)?
 * exprStmt    → expression ";"
 * expression  → equality
 * equality    → comparison (("!=" | "==") comparison)*
 * comparison  → term ((">" | ">=" | "<" | "<=") term)*
 * term        → factor (("+" | "-") factor)*
 * factor      → unary (("*" | "/") unary)*
 * unary       → ("!" | "-") unary | primary
 * primary     → NUMBER | STRING | "(" expression ")" | IDENTIFIER
 ******************************************************************************/

#include "parser.hpp"
#include <vector>
#include <iostream>

namespace pog {

    Parser::Parser(std::vector<Token> tokens)
        : tokens(std::move(tokens)) {}

    std::vector<std::unique_ptr<Statement>> Parser::parse() {
        std::vector<std::unique_ptr<Statement>> statements;

        while (!isAtEnd()) {
            statements.push_back(statement());
        }

        return statements;
    }

    std::unique_ptr<Statement> Parser::functionDeclaration() {
        Token name = consume(TokenType::IDENTIFIER, "Expect function name.");
        consume(TokenType::LEFT_PAREN, "Expect '(' after function name.");

        std::vector<Token> parameters;
        if (!check(TokenType::RIGHT_PAREN)) {
            do {
                if (parameters.size() >= 255) {
                    error(peek(), "Can't have more than 255 parameters.");
                }

                parameters.push_back(
                    consume(TokenType::IDENTIFIER, "Expect parameter name."));
            } while (match({ TokenType::COMMA }));
        }
        consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");

        consume(TokenType::LEFT_BRACE, "Expect '{' before function body.");
        std::vector<std::unique_ptr<Statement>> body;
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            body.push_back(statement());
        }
        consume(TokenType::RIGHT_BRACE, "Expect '}' after function body.");

        return std::make_unique<FunctionStmt>(std::move(name),
            std::move(parameters),
            std::move(body));
    }

    std::unique_ptr<Statement> Parser::ifStatement() {
        consume(TokenType::LEFT_PAREN, "Expect '(' after 'pepega'.");
        auto condition = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");

        auto thenBranch = statement();
        std::unique_ptr<Statement> elseBranch = nullptr;

        if (match({ TokenType::WEIRDCHAMP })) {
            elseBranch = statement();
        }

        return std::make_unique<IfStatement>(
            std::move(condition),
            std::move(thenBranch),
            std::move(elseBranch)
        );
    }

    std::unique_ptr<Statement> Parser::expressionStatement() {
        auto expr = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after expression.");
        return std::make_unique<ExpressionStmt>(std::move(expr));
    }

    std::unique_ptr<Expression> Parser::expression() {
        return assignment();  // Start with assignment now
    }

    std::unique_ptr<Expression> Parser::assignment() {
        auto expr = equality();

        if (match({ TokenType::EQUAL })) {
            Token equals = previous();
            auto value = assignment();  // Right-recursion for assignment

            if (auto* variable = dynamic_cast<Variable*>(expr.get())) {
                Token name = variable->name;
                return std::make_unique<AssignExpr>(std::move(name), std::move(value));
            }

            error(equals, "Invalid assignment target.");
        }

        return expr;
    }
    std::unique_ptr<Expression> Parser::equality() {
        auto expr = comparison();

        while (match({ TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL })) {
            Token op = previous();
            auto right = comparison();
            expr = std::make_unique<Binary>(
                std::move(expr),
                std::move(op),
                std::move(right)
            );
        }

        return expr;
    }

    std::unique_ptr<Expression> Parser::comparison() {
        auto expr = term();

        while (match({ TokenType::GREATER, TokenType::GREATER_EQUAL,
                     TokenType::LESS, TokenType::LESS_EQUAL })) {
            Token op = previous();
            auto right = term();
            expr = std::make_unique<Binary>(
                std::move(expr),
                std::move(op),
                std::move(right)
            );
        }

        return expr;
    }

    std::unique_ptr<Expression> Parser::term() {
        auto expr = factor();

        while (match({ TokenType::MINUS, TokenType::PLUS })) {
            Token op = previous();
            auto right = factor();
            expr = std::make_unique<Binary>(
                std::move(expr),
                std::move(op),
                std::move(right)
            );
        }

        return expr;
    }

    std::unique_ptr<Expression> Parser::factor() {
        auto expr = unary();

        while (match({ TokenType::SLASH, TokenType::STAR })) {
            Token op = previous();
            auto right = unary();
            expr = std::make_unique<Binary>(
                std::move(expr),
                std::move(op),
                std::move(right)
            );
        }

        return expr;
    }

    std::unique_ptr<Expression> Parser::unary() {
        if (match({ TokenType::BANG, TokenType::MINUS })) {
            Token op = previous();
            auto right = unary();
            return std::make_unique<Unary>(std::move(op), std::move(right));
        }

        return call();
    }

    std::unique_ptr<Expression> Parser::primary() {
        std::cout << "Entering primary()" << std::endl;
        std::cout << "Current token: " << peek().getLexeme() << std::endl;

        if (match({ TokenType::POGGERS })) {
            std::cout << "Matched POGGERS" << std::endl;
            Token name = previous();
            return std::make_unique<Variable>(std::move(name));
        }

        if (match({ TokenType::NUMBER, TokenType::STRING })) {
            std::cout << "Matched NUMBER/STRING" << std::endl;
            return std::make_unique<Literal>(previous().getLexeme());
        }

        if (match({ TokenType::IDENTIFIER })) {
            std::cout << "Matched IDENTIFIER" << std::endl;
            return std::make_unique<Variable>(previous());
        }

        if (match({ TokenType::LEFT_PAREN })) {
            std::cout << "Matched LEFT_PAREN" << std::endl;
            auto expr = expression();
            consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
            return std::make_unique<Grouping>(std::move(expr));
        }

        std::cout << "No match found in primary()" << std::endl;
        throw ParseError("Expect expression.");
    }

    // Helper methods
    bool Parser::match(std::initializer_list<TokenType> types) {
        for (TokenType type : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }
        return false;
    }

    bool Parser::check(TokenType type) const {
        if (isAtEnd()) return false;
        return peek().getType() == type;
    }

    const Token& Parser::advance() {
        if (!isAtEnd()) current++;
        return previous();
    }

    bool Parser::isAtEnd() const {
        return peek().getType() == TokenType::END_OF_FILE;
    }

    const Token& Parser::peek() const {
        return tokens[current];
    }

    const Token& Parser::previous() const {
        return tokens[current - 1];
    }

    const Token& Parser::consume(TokenType type, const std::string& message) {
        if (check(type)) return advance();
        throw ParseError(message);
    }

    Token Parser::error(const Token& token, const std::string& message) {
        throw ParseError(message);
    }

    std::unique_ptr<Expression> Parser::call() {
        std::cout << "Entering call()" << std::endl;
        auto expr = primary();

        while (true) {
            if (match({ TokenType::LEFT_PAREN })) {
                std::cout << "Found function call" << std::endl;
                expr = finishCall(std::move(expr));
            }
            else {
                break;
            }
        }

        return expr;
    }

    std::unique_ptr<Expression> Parser::finishCall(std::unique_ptr<Expression> callee) {
        std::vector<std::unique_ptr<Expression>> arguments;

        if (!check(TokenType::RIGHT_PAREN)) {
            do {
                if (arguments.size() >= 255) {
                    error(peek(), "Can't have more than 255 arguments.");
                }
                arguments.push_back(expression());
            } while (match({ TokenType::COMMA }));
        }

        Token paren = consume(TokenType::RIGHT_PAREN,
            "Expect ')' after arguments.");

        return std::make_unique<CallExpression>(
            std::move(callee),
            std::move(paren),
            std::move(arguments));
    }

    std::unique_ptr<Statement> Parser::statement() {
        if (match({ TokenType::POGCHAMP })) {
            return functionDeclaration();
        }
        if (match({ TokenType::PEPEGA })) {
            return ifStatement();
        }
        if (match({ TokenType::KAPPA })) {
            return whileStatement();
        }
        if (match({ TokenType::LEFT_BRACE })) {
            return block();
        }

        return expressionStatement();
    }

    std::unique_ptr<Statement> Parser::block() {
        std::vector<std::unique_ptr<Statement>> statements;

        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            statements.push_back(statement());
        }

        consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
        return std::make_unique<BlockStmt>(std::move(statements));
    }

    std::unique_ptr<Statement> Parser::whileStatement() {
        consume(TokenType::LEFT_PAREN, "Expect '(' after 'kappa'.");
        auto condition = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
        auto body = statement();

        return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
    }

} // namespace pog