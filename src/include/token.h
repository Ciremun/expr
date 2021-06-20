#ifndef TOKEN_H
#define TOKEN_H

#include <variant>
#include <string>

#include "kind.h"
#include "typedef.h"

struct Token
{
    using Value = std::variant<size, int, float, bool, const char *, std::string>;

    Kind kind = Kind::error_token;
    size position = 0;
    std::string text;
    Value value;

    Token();
    Token(Kind kind, size position, std::string text, Value value);
};

#endif // TOKEN_H