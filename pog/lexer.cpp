/*******************************************************************************
 * POG Language Interpreter
 * lexer.cpp
 *
 * Implementation of the Lexer class that performs tokenization of POG source code.
 *
 * Features:
 * - Processes single and multi-character tokens
 * - Handles string literals
 * - Recognizes keywords
 * - Tracks line numbers
 * - Implements lookahead for multi-character tokens
 * - Manages whitespace and comments
 *
 * The lexer reads characters one at a time and groups them into tokens based
 * on the POG language specification.
 ******************************************************************************/

#include "lexer.hpp"
#include <unordered_map>

namespace pog {

    static const std::unordered_map<std::string, TokenType> keywords = {
        {"poggers", TokenType::POGGERS},
        {"pepega", TokenType::PEPEGA},
        {"weirdchamp", TokenType::WEIRDCHAMP},
        {"pogchamp", TokenType::POGCHAMP},
        {"kappa", TokenType::KAPPA},
        {"monkas", TokenType::MONKAS}
    };

    Lexer::Lexer(std::string source)
        : source(std::move(source)) {}

    std::vector<Token> Lexer::scanTokens() {
        while (!isAtEnd()) {
            start = current;
            scanToken();
        }

        tokens.emplace_back(TokenType::END_OF_FILE, "", line);
        return tokens;
    }
    void Lexer::addToken(TokenType type) {
        std::string_view text = std::string_view(source).substr(start, current - start);
        tokens.emplace_back(type, text, line);
    }

    void Lexer::addToken(TokenType type, std::string_view lexeme) {
        tokens.emplace_back(type, lexeme, line);
    }

    void Lexer::scanToken() {
        char c = advance();
        switch (c) {
            // Single-character tokens
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*': addToken(TokenType::STAR); break;

            // Two-character tokens
        case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
        case '/':
            if (match('+')) {
                // Comment goes until +/
                while (!(peek() == '+' && peekNext() == '/') && !isAtEnd()) {
                    if (peek() == '\n') line++;
                    advance();
                }

                if (!isAtEnd()) {
                    // Consume the +/
                    advance(); // +
                    advance(); // /
                }
            }
            else {
                addToken(TokenType::SLASH);
            }
            break;
            // Handle whitespace
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;

            // String literals
        case '"': string(); break;

        default:
            if (isDigit(c)) {
                number();
            }
            else if (isAlpha(c)) {
                identifier();
            }
            else {
                // Handle error - unexpected character
            }
            break;
        }
    }

    bool Lexer::isAtEnd() const {
        return current >= source.length();
    }

    char Lexer::advance() {
        return source[current++];
    }

    bool Lexer::match(char expected) {
        if (isAtEnd() || source[current] != expected) {
            return false;
        }
        current++;
        return true;
    }

    char Lexer::peek() const {
        if (isAtEnd()) return '\0';
        return source[current];
    }

    char Lexer::peekNext() const {
        if (current + 1 >= source.length()) return '\0';
        return source[current + 1];
    }

    bool Lexer::isDigit(char c) const {
        return c >= '0' && c <= '9';
    }

    bool Lexer::isAlpha(char c) const {
        return (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '_';
    }

    bool Lexer::isAlphaNumeric(char c) const {
        return isAlpha(c) || isDigit(c);
    }

    void Lexer::number() {
        while (isDigit(peek())) advance();

        // Look for decimal point followed by more digits
        if (peek() == '.' && isDigit(peekNext())) {
            advance(); // Consume the '.'
            while (isDigit(peek())) advance();
        }

        std::string_view number = std::string_view(source).substr(start, current - start);
        addToken(TokenType::NUMBER, number);
    }

    void Lexer::string() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') line++;
            advance();
        }

        if (isAtEnd()) {
            // Handle error: unterminated string
            return;
        }

        // Consume the closing "
        advance();

        // Trim the surrounding quotes
        std::string_view string = std::string_view(source).substr(start + 1, current - start - 2);
        addToken(TokenType::STRING, string);
    }

    void Lexer::identifier() {
        while (isAlphaNumeric(peek())) advance();

        std::string_view text = std::string_view(source).substr(start, current - start);

        // Check if the identifier is a keyword
        auto it = keywords.find(std::string(text));
        TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;

        addToken(type, text);
    }
}