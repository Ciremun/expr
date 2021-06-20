#include "expression.h"

NumberExpr::NumberExpr(Token number)
    : kind(Kind::number_expr), number(number)
{
}

BinaryExpr::BinaryExpr(Expression* left, Token op, Expression* right)
    : left(left), op(op), right(right), kind(Kind::binary_expr)
{
}

ParenExpr::ParenExpr(Token open_paren, Expression* expr, Token close_paren)
    : open_paren(open_paren), expr(expr), close_paren(close_paren), kind(Kind::paren_expr)
{
}