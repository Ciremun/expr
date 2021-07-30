#ifndef TOKEN_H
#define TOKEN_H

#include <string>

#include "text_span.h"
#include "typedef.h"

struct Token {
    Kind kind = Kind::error_token;
    size position = 0;
    std::string text;
    Value value;
    TextSpan* span;

    Token();
    Token(Kind kind, size position, std::string text, Value value);
};

#endif // TOKEN_H
