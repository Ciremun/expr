#ifndef KIND_H
#define KIND_H

enum Kind : int {
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
    identifier_token,
    bang_token,
    double_ampersand_token,
    double_pipe_token,

    literal_expr,
    unary_expr,
    binary_expr,
    paren_expr,

    true_keyword,
    false_keyword,

    count
};

constexpr const char *kinds[] = {
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
    "identifier_token",
    "bang_token",
    "double_ampersand_token",
    "double_pipe_token",
    "literal_expr",
    "unary_expr",
    "binary_expr",
    "paren_expr",
    "true_keyword",
    "false_keyword"
};

static_assert(sizeof(kinds) / sizeof(kinds[0]) == Kind::count, "update kinds[]");

enum class BoundNodeKind {
    literal_expr,
    unary_expr,
    binary_expr
};

enum class BoundUnaryOperatorKind {
    Identity,
    Negation,
    LogicalNegation,
    Error
};

enum class BoundBinaryOperatorKind {
    Addition,
    Subtraction,
    Multiplication,
    Division,
    LogicalAnd,
    LogicalOr,
    Error
};

#endif // KIND_H