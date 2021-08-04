#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "kind.h"
#include "token.h"
#include "typedef.h"

struct Node {
    Kind kind = Kind::error_token;
};

struct Expression : Node {
    virtual ~Expression() = default;
};

struct LiteralExpr : Expression {
    Token literal;
    Value value;
    Kind kind = Kind::literal_expr;

    LiteralExpr(Token literal);
    LiteralExpr(Token literal, Value value);
};

struct BinaryExpr : Expression {
    Expression *left;
    Token op;
    Expression *right;
    Kind kind = Kind::binary_expr;

    BinaryExpr(Expression *left, Token op, Expression *right);
};

struct UnaryExpr : Expression {
    Token op;
    Expression *operand;
    Kind kind = Kind::unary_expr;

    UnaryExpr(Token op, Expression *operand);
};

struct ParenExpr : Expression {
    Token open_paren;
    Expression *expr;
    Token close_paren;
    Kind kind = Kind::paren_expr;

    ParenExpr(Token open_paren, Expression *expr, Token close_paren);
};

struct NameExpr : Expression {
    Token identifier;
    Kind kind = Kind::name_expr;

    NameExpr(Token identifier);
};

struct AssignmentExpr : Expression {
    Token identifier;
    Token equals;
    Expression *expr;
    Kind kind = Kind::assignment_expr;

    AssignmentExpr(Token identifier, Token equals, Expression *expr);
};

#endif // EXPRESSION_H
