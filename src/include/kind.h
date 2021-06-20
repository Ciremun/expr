#ifndef KIND_H
#define KIND_H

enum Kind: int
{
    eof,
    error,
    number,
    space,
    open_paren,
    close_paren,

    plus,
    minus,
    star,
    forward_slash,

    literal_expr,
    binary_expr,
    paren_expr
};

constexpr const char* kinds[] = {
    "eof",
    "error",
    "number",
    "space",
    "open_paren",
    "close_paren",

    "plus",
    "minus",
    "star",
    "forward_slash",

    "literal_expr",
    "binary_expr",
    "paren_expr"
};

#endif // KIND_H