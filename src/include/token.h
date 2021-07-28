#ifndef TOKEN_H
#define TOKEN_H

#include <string>

#include "kind.h"
#include "typedef.h"

struct Token {
    Kind        kind = Kind::error_token;
    size        position = 0;
    std::string text;
    Value       value;

    Token();
    Token(Kind kind, size position, std::string text, Value value);
};

#endif // TOKEN_H