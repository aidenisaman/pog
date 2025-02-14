/*******************************************************************************
 * POG Language Interpreter
 * token.hpp
 *
 * Defines the Token class and TokenType enum which represent the basic units
 * of the POG language. Each token has a type, the actual text (lexeme), and
 * line number where it appears in the source code.
 *
 * Token Types include:
 * - Keywords (poggers, pepega, etc.)
 * - Operators (+, -, *, /, etc.)
 * - Literals (numbers, strings)
 * - Special tokens (EOF)
 ******************************************************************************/
#pragma once
#include <string>
#include <string_view>

namespace pog {

    enum class TokenType {
        // Keywords
        POGGERS,    // print
        PEPEGA,     // if
        WEIRDCHAMP, // else
        POGCHAMP,   // function
        KAPPA,      // while
        MONKAS,     // try

        // Single-character tokens
        LEFT_PAREN, RIGHT_PAREN,     // ( )
        LEFT_BRACE, RIGHT_BRACE,     // { }
        COMMA, DOT, MINUS, PLUS,     // , . - +
        SEMICOLON, SLASH, STAR,      // ; / *

        // One or two character tokens
        BANG, BANG_EQUAL,          // ! !=
        EQUAL, EQUAL_EQUAL,        // = ==
        GREATER, GREATER_EQUAL,    // > >=
        LESS, LESS_EQUAL,         // < <=

        FUNCTION,

        // Literals
        IDENTIFIER, STRING, NUMBER,

        // Special tokens
        END_OF_FILE
    };

    class Token {
    public:
        // Single constructor that takes string_view
        Token(TokenType type, std::string_view lexeme, int line);

        TokenType getType() const { return type; }
        const std::string& getLexeme() const { return lexeme; }
        int getLine() const { return line; }

    private:
        TokenType type;
        std::string lexeme;
        int line;
    };

} // namespace pog