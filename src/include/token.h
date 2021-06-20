#ifndef TOKEN_H
#define TOKEN_H

#include <variant>
#include <string>

#include "kind.h"

struct Token
{
    using Value = std::variant<size_t, int, float, bool, const char *, std::string>;

    Kind kind = Kind::error;
    size_t position = 0;
    std::string text;
    Value value;

    Token();
    Token(Kind kind, size_t position, std::string text, Value value);
};

#endif // TOKEN_H