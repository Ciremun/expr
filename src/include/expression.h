#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "kind.h"
#include "token.h"

struct Node
{
    Kind kind = Kind::error_token;
};

struct Expression : Node
{
    virtual ~Expression() = default;
};

struct LiteralExpr : Expression
{
    Kind kind;
    Token literal;

    LiteralExpr(Token literal);
};

struct BinaryExpr : Expression
{
    Expression* left;
    Token op;
    Expression* right;
    Kind kind;

    BinaryExpr(Expression* left, Token op, Expression* right);
};

struct ParenExpr : Expression
{
    Token open_paren;
    Expression* expr;
    Token close_paren;
    Kind kind;

    ParenExpr(Token open_paren, Expression* expr, Token close_paren);
};

#endif // EXPRESSION_H