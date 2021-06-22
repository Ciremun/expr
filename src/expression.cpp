#include "expression.h"

LiteralExpr::LiteralExpr(Token literal)
    : literal(literal)
{
}

BinaryExpr::BinaryExpr(Expression* left, Token op, Expression* right)
    : left(left), op(op), right(right)
{
}

ParenExpr::ParenExpr(Token open_paren, Expression* expr, Token close_paren)
    : open_paren(open_paren), expr(expr), close_paren(close_paren)
{
}

UnaryExpr::UnaryExpr(Token op, Expression* operand)
    : op(op), operand(operand)
{
}