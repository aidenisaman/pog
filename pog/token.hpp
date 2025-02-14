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

        // Literals
        IDENTIFIER, STRING, NUMBER,

        // Special tokens
        END_OF_FILE
    };

    class Token {
    public:
        Token(TokenType type, std::string lexeme, int line);
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
