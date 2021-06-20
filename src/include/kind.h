#ifndef KIND_H
#define KIND_H

enum Kind: int
{
    eof_token,
    error_token,
    number_token,
    space_token,
    open_paren_token,
    close_paren_token,
    plus_token,
    minus_token,
    star_token,
    forward_slash_token,

    literal_expr,
    binary_expr,
    paren_expr
};

constexpr const char* kinds[] = {
    "eof_token",
    "error_token",
    "number_token",
    "space_token",
    "open_paren_token",
    "close_paren_token",
    "plus_token",
    "minus_token",
    "star_token",
    "forward_slash_token",

    "literal_expr",
    "binary_expr",
    "paren_expr"
};

#endif // KIND_H