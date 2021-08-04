#include "expression.h"

Expression::Expression(Kind kind)
    : kind(kind) {}

LiteralExpr::LiteralExpr(Token literal)
    : Expression(Kind::literal_expr), literal(literal), value(literal.value) {}

LiteralExpr::LiteralExpr(Token literal, Value value)
    : Expression(Kind::literal_expr), literal(literal), value(value) {}

BinaryExpr::BinaryExpr(Expression *left, Token op, Expression *right)
    : Expression(Kind::binary_expr), left(left), op(op), right(right) {}

ParenExpr::ParenExpr(Token open_paren, Expression *expr, Token close_paren)
    : Expression(Kind::paren_expr), open_paren(open_paren), expr(expr), close_paren(close_paren) {}

UnaryExpr::UnaryExpr(Token op, Expression *operand)
    : Expression(Kind::unary_expr), op(op), operand(operand) {}

NameExpr::NameExpr(Token identifier)
    : Expression(Kind::name_expr), identifier(identifier) {}

AssignmentExpr::AssignmentExpr(Token identifier, Token equals, Expression *expr)
    : Expression(Kind::assignment_expr), identifier(identifier), equals(equals), expr(expr) {}
