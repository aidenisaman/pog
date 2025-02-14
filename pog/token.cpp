#include "token.hpp"

namespace pog {

    Token::Token(TokenType type, std::string lexeme, int line)
        : type(type)
        , lexeme(std::move(lexeme))
        , line(line)
    {
    }

    Token::Token(TokenType type, std::string_view lexeme, int line)
        : type(type)
        , lexeme(std::string(lexeme))
        , line(line)
    {
    }

} // namespace pog