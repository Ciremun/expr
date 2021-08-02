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
    equals_token,
    double_ampersand_token,
    double_pipe_token,
    double_equals_token,
    bang_equals_token,

    literal_expr,
    unary_expr,
    binary_expr,
    paren_expr,
    name_expr,
    assignment_expr,

    true_keyword,
    false_keyword,

    count
};

const char *const kinds[] = {
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
    "equals_token",
    "double_ampersand_token",
    "double_pipe_token",
    "double_equals_token",
    "bang_equals_token",
    "literal_expr",
    "unary_expr",
    "binary_expr",
    "paren_expr",
    "name_expr",
    "assignment_expr",
    "true_keyword",
    "false_keyword",
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
};

enum class BoundBinaryOperatorKind {
    Addition,
    Subtraction,
    Multiplication,
    Division,
    LogicalAnd,
    LogicalOr,
    Equals,
    NotEquals
};

#endif // KIND_H
