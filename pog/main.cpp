/*******************************************************************************
 * POG Language Interpreter
 * main.cpp
 *
 * Entry point for the POG interpreter. Tests both lexer and parser by:
 * 1. Tokenizing sample POG code
 * 2. Parsing tokens into an AST
 * 3. Displaying both tokens and parsed structure
 ******************************************************************************/

#include "lexer.hpp"
#include "parser.hpp"
#include <iostream>
#include <string>
#include <memory>

 // Helper function to convert TokenType to string
std::string tokenTypeToString(pog::TokenType type) {
    switch (type) {
    case pog::TokenType::POGGERS: return "POGGERS";
    case pog::TokenType::PEPEGA: return "PEPEGA";
    case pog::TokenType::WEIRDCHAMP: return "WEIRDCHAMP";
    case pog::TokenType::POGCHAMP: return "POGCHAMP";
    case pog::TokenType::KAPPA: return "KAPPA";
    case pog::TokenType::MONKAS: return "MONKAS";
    case pog::TokenType::LEFT_PAREN: return "LEFT_PAREN";
    case pog::TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
    case pog::TokenType::LEFT_BRACE: return "LEFT_BRACE";
    case pog::TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
    case pog::TokenType::COMMA: return "COMMA";
    case pog::TokenType::DOT: return "DOT";
    case pog::TokenType::MINUS: return "MINUS";
    case pog::TokenType::PLUS: return "PLUS";
    case pog::TokenType::SEMICOLON: return "SEMICOLON";
    case pog::TokenType::SLASH: return "SLASH";
    case pog::TokenType::STAR: return "STAR";
    case pog::TokenType::BANG: return "BANG";
    case pog::TokenType::BANG_EQUAL: return "BANG_EQUAL";
    case pog::TokenType::EQUAL: return "EQUAL";
    case pog::TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
    case pog::TokenType::GREATER: return "GREATER";
    case pog::TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
    case pog::TokenType::LESS: return "LESS";
    case pog::TokenType::LESS_EQUAL: return "LESS_EQUAL";
    case pog::TokenType::IDENTIFIER: return "IDENTIFIER";
    case pog::TokenType::STRING: return "STRING";
    case pog::TokenType::NUMBER: return "NUMBER";
    case pog::TokenType::END_OF_FILE: return "EOF";
    default: return "OTHER";
    }
}

// Forward declare printExpression since printStatement needs it
void printExpression(const pog::Expression* expr, int indent = 0);

void printStatement(const pog::Statement* stmt, int indent = 0) {
    if (!stmt) return;

    std::string indentation(indent * 2, ' ');

    if (const auto* exprStmt = dynamic_cast<const pog::ExpressionStmt*>(stmt)) {
        std::cout << indentation << "Expression Statement:" << std::endl;
        printExpression(exprStmt->expression.get(), indent + 1);
    }
    else if (const auto* ifStmt = dynamic_cast<const pog::IfStatement*>(stmt)) {
        std::cout << indentation << "If Statement:" << std::endl;
        std::cout << indentation << "Condition:" << std::endl;
        printExpression(ifStmt->condition.get(), indent + 1);
        std::cout << indentation << "Then:" << std::endl;
        printStatement(ifStmt->thenBranch.get(), indent + 1);
        if (ifStmt->elseBranch) {
            std::cout << indentation << "Else:" << std::endl;
            printStatement(ifStmt->elseBranch.get(), indent + 1);
        }
    }
    else if (const auto* whileStmt = dynamic_cast<const pog::WhileStmt*>(stmt)) {
        std::cout << indentation << "While Statement:" << std::endl;
        std::cout << indentation << "Condition:" << std::endl;
        printExpression(whileStmt->condition.get(), indent + 1);
        std::cout << indentation << "Body:" << std::endl;
        printStatement(whileStmt->body.get(), indent + 1);
    }
    else if (const auto* funcStmt = dynamic_cast<const pog::FunctionStmt*>(stmt)) {
        std::cout << indentation << "Function Declaration: " << funcStmt->name.getLexeme() << std::endl;
        std::cout << indentation << "Parameters: ";
        for (const auto& param : funcStmt->parameters) {
            std::cout << param.getLexeme() << " ";
        }
        std::cout << std::endl;
        std::cout << indentation << "Body:" << std::endl;
        for (const auto& bodyStmt : funcStmt->body) {
            printStatement(bodyStmt.get(), indent + 1);
        }
    }
    else if (const auto* blockStmt = dynamic_cast<const pog::BlockStmt*>(stmt)) {
        std::cout << indentation << "Block:" << std::endl;
        for (const auto& stmt : blockStmt->statements) {
            printStatement(stmt.get(), indent + 1);
        }
    }
}

void printExpression(const pog::Expression* expr, int indent) {
    if (!expr) return;

    std::string indentation(indent * 2, ' ');

    if (const auto* literal = dynamic_cast<const pog::Literal*>(expr)) {
        std::cout << indentation << "Literal: " << literal->value << std::endl;
    }
    else if (const auto* binary = dynamic_cast<const pog::Binary*>(expr)) {
        std::cout << indentation << "Binary: " << binary->op.getLexeme() << std::endl;
        std::cout << indentation << "Left:" << std::endl;
        printExpression(binary->left.get(), indent + 1);
        std::cout << indentation << "Right:" << std::endl;
        printExpression(binary->right.get(), indent + 1);
    }
    else if (const auto* unary = dynamic_cast<const pog::Unary*>(expr)) {
        std::cout << indentation << "Unary: " << unary->op.getLexeme() << std::endl;
        printExpression(unary->right.get(), indent + 1);
    }
    else if (const auto* variable = dynamic_cast<const pog::Variable*>(expr)) {
        std::cout << indentation << "Variable: " << variable->name.getLexeme() << std::endl;
    }
    else if (const auto* grouping = dynamic_cast<const pog::Grouping*>(expr)) {
        std::cout << indentation << "Grouping:" << std::endl;
        printExpression(grouping->expression.get(), indent + 1);
    }
    else if (const auto* call = dynamic_cast<const pog::CallExpression*>(expr)) {
        std::cout << indentation << "Call:" << std::endl;
        std::cout << indentation << "Callee:" << std::endl;
        printExpression(call->callee.get(), indent + 1);
        std::cout << indentation << "Arguments:" << std::endl;
        for (const auto& arg : call->arguments) {
            printExpression(arg.get(), indent + 1);
        }
    }
}

int main() {
    // Test program with all features
    std::string source = R"(
        pogchamp main() {
            poggers("Starting program!");
            
            // If statement test
            pepega (x > 10) {
                poggers("x is greater than 10!");
            } weirdchamp {
                poggers("x is not greater than 10!");
            }

            // While loop test
            kappa (count < 3) {
                poggers("Counting...");
                count = count + 1;
            }

            // Nested structures test
            pepega (flag) {
                kappa (y > 0) {
                    poggers("Processing...");
                    y = y - 1;
                }
            }
        }
    )";

    // Lexical analysis
    std::cout << "=== Tokens ===" << std::endl;
    pog::Lexer lexer(source);
    auto tokens = lexer.scanTokens();

    for (const auto& token : tokens) {
        std::cout << "Line " << token.getLine()
            << " | Type: " << tokenTypeToString(token.getType())
            << " | Lexeme: '" << token.getLexeme() << "'"
            << std::endl;
    }

    // Parsing
    std::cout << "\n=== Parse Tree ===" << std::endl;
    pog::Parser parser(tokens);
    try {
        auto statements = parser.parse();
        for (const auto& stmt : statements) {
            printStatement(stmt.get());
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
    }

    return 0;
}