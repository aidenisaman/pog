/*******************************************************************************
 * POG Language Interpreter
 * token.cpp
 *
 * Implementation of the Token class constructors. Supports creation of tokens
 * from both std::string and std::string_view for efficiency.
 *
 * The string_view constructor is particularly useful when tokenizing source code
 * as it avoids unnecessary string copies for substrings of the source.
 ******************************************************************************/
#include "token.hpp"

#include "token.hpp"

namespace pog {

    Token::Token(TokenType type, std::string_view lexeme, int line)
        : type(type)
        , lexeme(std::string(lexeme))
        , line(line)
    {
    }

} // namespace pog