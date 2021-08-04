#include "expression.h"

LiteralExpr::LiteralExpr(Token literal)
    : literal(literal), value(literal.value) {}

LiteralExpr::LiteralExpr(Token literal, Value value)
    : literal(literal), value(value) {}

BinaryExpr::BinaryExpr(Expression *left, Token op, Expression *right)
    : left(left), op(op), right(right) {}

ParenExpr::ParenExpr(Token open_paren, Expression *expr, Token close_paren)
    : open_paren(open_paren), expr(expr), close_paren(close_paren) {}

UnaryExpr::UnaryExpr(Token op, Expression *operand)
    : op(op), operand(operand) {}

NameExpr::NameExpr(Token identifier)
    : identifier(identifier) {}

AssignmentExpr::AssignmentExpr(Token identifier, Token equals, Expression *expr)
    : identifier(identifier), equals(equals), expr(expr) {}
