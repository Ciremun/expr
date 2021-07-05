#include "binder.h"
#include "kind.h"
#include "util.h"

BoundUnaryExpr::BoundUnaryExpr(BoundUnaryOperatorKind op_kind, BoundExpr* operand)
    : op_kind(op_kind), operand(operand), type(operand->type)
{
}

BoundLiteralExpr::BoundLiteralExpr(Value value, size_t type)
    : value(value), type(type)
{
}

BoundBinaryExpr::BoundBinaryExpr(BoundExpr* left, BoundBinaryOperatorKind op_kind, BoundExpr* right)
    : left(left), op_kind(op_kind), right(right), type(left->type)
{
}

BoundExpr* Binder::bind_literal_expr(LiteralExpr* syntax)
{
    printf("bind_literal_expr index %lld\n", syntax->value.index());
    return new BoundLiteralExpr(syntax->value, syntax->value.index());
}

BoundExpr* Binder::bind_unary_expr(UnaryExpr* syntax)
{
    BoundExpr* bound_operand = bind_expr(syntax->operand);
    BoundUnaryOperatorKind bound_operator_kind = bind_unary_operator_kind(syntax->op.kind, bound_operand->type);
    if (bound_operator_kind == BoundUnaryOperatorKind::Error) {
        errors.push_back(format("Unary operator <%s> is not defined for type <%s>", syntax->op.text.c_str(), "number"));
        return bound_operand;
    }
    return new BoundUnaryExpr(bound_operator_kind, bound_operand);
}

BoundExpr* Binder::bind_binary_expr(BinaryExpr* syntax)
{
    BoundExpr* bound_left = bind_expr(syntax->left);
    BoundExpr* bound_right = bind_expr(syntax->right);
    printf("bind_binary_expr index %lld %lld\n", bound_left->type, bound_right->type);
    BoundBinaryOperatorKind bound_operator_kind = bind_binary_operator_kind(syntax->op.kind, bound_left->type, bound_right->type);
    if (bound_operator_kind == BoundBinaryOperatorKind::Error) {
        errors.push_back(format("Binary operator <%s> is not defined for types <%s> and <%s>", syntax->op.text.c_str(), "number", "number"));
        return bound_left;
    }
    return new BoundBinaryExpr(bound_left, bound_operator_kind, bound_right);
}

BoundExpr* Binder::bind_expr(Expression* syntax)
{
    if (LiteralExpr *literal_expr = dynamic_cast<LiteralExpr*>(syntax)) return bind_literal_expr(literal_expr);
    if (UnaryExpr   *unary_expr   = dynamic_cast<UnaryExpr*>(syntax))   return bind_unary_expr(unary_expr);
    if (BinaryExpr  *binary_expr  = dynamic_cast<BinaryExpr*>(syntax))  return bind_binary_expr(binary_expr);
    runtime_error("Unexpected syntax <%s>\n", kinds[syntax->kind]);
}

BoundUnaryOperatorKind Binder::bind_unary_operator_kind(Kind kind, size_t op_type)
{
    printf("uwu %lld\n", op_type);
    if (op_type != variant_type::size_vt) {
        return BoundUnaryOperatorKind::Error;
    }
    switch (kind) {
    case Kind::plus_token:
        return BoundUnaryOperatorKind::Identity;
    case Kind::minus_token:
        return BoundUnaryOperatorKind::Negation;
    default:
        runtime_error("Unexpected unary operator <%s>\n", kinds[kind]);
    }
}

BoundBinaryOperatorKind Binder::bind_binary_operator_kind(Kind kind, size_t left_type, size_t right_type)
{
    printf("owo %lld %lld\n", left_type, right_type);
    if (left_type != variant_type::size_vt || right_type != variant_type::size_vt) {
        return BoundBinaryOperatorKind::Error;
    }
    switch (kind) {
    case Kind::plus_token:
        return BoundBinaryOperatorKind::Addition;
    case Kind::minus_token:
        return BoundBinaryOperatorKind::Subtraction;
    case Kind::star_token:
        return BoundBinaryOperatorKind::Multiplication;
    case Kind::forward_slash_token:
        return BoundBinaryOperatorKind::Division;
    default:
        runtime_error("Unexpected binary operator <%s>\n", kinds[kind]);
    }
}
