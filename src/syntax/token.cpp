#include "token.h"
#include "kind.h"

Token::Token() {}

Token::Token(Kind kind, size position, std::string text, Value value)
    : kind(kind), position(position), text(text), value(value), span(new TextSpan(position, text.length())) {}
