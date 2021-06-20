#ifndef KIND_H
#define KIND_H

enum Kind: int
{
    number,
    space,
    plus,
    minus,
    star,
    forward_slash,
    open_paren,
    close_parent,
    error,
    eof,
    number_expr,
    binary_expr,
    paren_expr
};

constexpr const char* kinds[] = {
    "number",
    "space",
    "plus",
    "minus",
    "star",
    "forward_slash",
    "open_paren",
    "close_parent" ,
    "error",
    "eof",
    "number_expr",
    "binary_expr",
    "paren_expr"
};

#endif // KIND_H