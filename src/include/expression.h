#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "kind.h"
#include "token.h"

struct Node {
    Kind kind = Kind::error_token;
};

struct Expression : Node {
    virtual ~Expression() = default;
};

struct LiteralExpr : Expression {
    Token literal;
    Kind  kind = Kind::literal_expr;
    Value value;

    LiteralExpr(Token literal);
    LiteralExpr(Token literal, Value value);
};

struct BinaryExpr : Expression {
    Expression *left;
    Token       op;
    Expression *right;
    Kind        kind = Kind::binary_expr;

    BinaryExpr(Expression *left, Token op, Expression *right);
};

struct UnaryExpr : Expression {
    Token       op;
    Expression *operand;
    Kind        kind = Kind::unary_expr;

    UnaryExpr(Token op, Expression *operand);
};

struct ParenExpr : Expression {
    Token       open_paren;
    Expression *expr;
    Token       close_paren;
    Kind        kind = Kind::paren_expr;

    ParenExpr(Token open_paren, Expression *expr, Token close_paren);
};

#endif // EXPRESSION_H