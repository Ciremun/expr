#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "kind.h"
#include "token.h"
#include "typedef.h"

struct Expression {
    Kind kind;

    Expression(Kind kind);
    virtual ~Expression() = default;
};

struct LiteralExpr : Expression {
    Token literal;
    Value value;

    LiteralExpr(Token literal);
    LiteralExpr(Token literal, Value value);
};

struct BinaryExpr : Expression {
    Expression *left;
    Token op;
    Expression *right;

    BinaryExpr(Expression *left, Token op, Expression *right);
};

struct UnaryExpr : Expression {
    Token op;
    Expression *operand;

    UnaryExpr(Token op, Expression *operand);
};

struct ParenExpr : Expression {
    Token open_paren;
    Expression *expr;
    Token close_paren;

    ParenExpr(Token open_paren, Expression *expr, Token close_paren);
};

struct NameExpr : Expression {
    Token identifier;

    NameExpr(Token identifier);
};

struct AssignmentExpr : Expression {
    Token identifier;
    Token equals;
    Expression *expr;

    AssignmentExpr(Token identifier, Token equals, Expression *expr);
};

#endif // EXPRESSION_H
