#pragma once
#include "token.hpp"
#include "ast.hpp"
#include <vector>
#include <memory>
#include <stdexcept>

namespace pog {

    class Parser {
    public:
        explicit Parser(std::vector<Token> tokens);
        std::vector<std::unique_ptr<Statement>> parse();

    private:
        // Parsing methods for different syntax constructs
        std::unique_ptr<Statement> statement();
        std::unique_ptr<Statement> expressionStatement();
        std::unique_ptr<Statement> ifStatement();
        std::unique_ptr<Statement> functionDeclaration();
        std::unique_ptr<Expression> finishCall(std::unique_ptr<Expression> callee);

        std::unique_ptr<Expression> expression();
        std::unique_ptr<Expression> equality();
        std::unique_ptr<Expression> comparison();
        std::unique_ptr<Expression> term();
        std::unique_ptr<Expression> factor();
        std::unique_ptr<Expression> unary();
        std::unique_ptr<Expression> call();
        std::unique_ptr<Expression> primary();
        std::unique_ptr<Statement> block();
        std::unique_ptr<Statement> whileStatement();
        std::unique_ptr<Expression> assignment();

        // Helper methods
        bool match(std::initializer_list<TokenType> types);
        bool check(TokenType type) const;
        const Token& advance();
        bool isAtEnd() const;
        const Token& peek() const;
        const Token& previous() const;
        const Token& consume(TokenType type, const std::string& message);
        Token error(const Token& token, const std::string& message);
        class ParseError : public std::runtime_error {
        public:
            explicit ParseError(const std::string& message)
                : std::runtime_error(message) {}
        };

        std::vector<Token> tokens;
        int current = 0;  // Current token index
    };



} // namespace pog