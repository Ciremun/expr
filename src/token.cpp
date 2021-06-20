#include "token.h"
#include "kind.h"

Token::Token()
{
}

Token::Token(Kind kind, size_t position, std::string text, Value value)
    : kind(kind), position(position), text(text), value(value)
{
}