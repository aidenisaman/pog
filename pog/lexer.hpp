/*******************************************************************************
 * POG Language Interpreter
 * lexer.hpp
 *
 * The Lexer class is responsible for breaking down source code into tokens.
 * It performs lexical analysis by scanning through the source code character
 * by character and grouping them into meaningful tokens.
 *
 * Key responsibilities:
 * - Tokenize source code into a sequence of tokens
 * - Handle different token types (keywords, operators, literals)
 * - Track line numbers for error reporting
 * - Identify and process language-specific keywords
 ******************************************************************************/
#pragma once
#pragma once
#include "token.hpp"
#include <string>
#include <vector>

namespace pog {

    class Lexer {
    public:
        explicit Lexer(std::string source);
        std::vector<Token> scanTokens();

    private:
        bool isAtEnd() const;
        void scanToken();
        char advance();
        bool match(char expected);
        char peek() const;
        char peekNext() const;
        void addToken(TokenType type);
        void addToken(TokenType type, std::string_view lexeme);
        bool isDigit(char c) const;
        bool isAlpha(char c) const;
        bool isAlphaNumeric(char c) const;
        void number();
        void identifier();
        void string();

        const std::string source;
        std::vector<Token> tokens;
        int start = 0;      // Start of current lexeme
        int current = 0;    // Current character position
        int line = 1;       // Current line number
    };

} // namespace pog