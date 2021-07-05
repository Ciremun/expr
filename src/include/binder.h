#ifndef BINDER_H
#define BINDER_H

#include <vector>
#include <string>

#include "expression.h"
#include "typedef.h"

struct BoundNode {
    BoundNodeKind kind;
};

struct BoundExpr : BoundNode {
    size_t type;

    virtual ~BoundExpr() = default;
};

struct BoundLiteralExpr : BoundExpr {
    Value value;
    BoundNodeKind kind = BoundNodeKind::literal_expr;
    size_t type;

    BoundLiteralExpr(Value value, size_t type);
};

struct BoundUnaryExpr : BoundExpr {
    BoundUnaryOperatorKind op_kind;
    BoundExpr *operand;
    BoundNodeKind kind = BoundNodeKind::unary_expr;
    size_t type;

    BoundUnaryExpr(BoundUnaryOperatorKind op_kind, BoundExpr *operand);
};

struct BoundBinaryExpr : BoundExpr {
    BoundExpr *left;
    BoundBinaryOperatorKind op_kind;
    BoundExpr *right;
    BoundNodeKind kind = BoundNodeKind::binary_expr;
    size_t type;

    BoundBinaryExpr(BoundExpr *left, BoundBinaryOperatorKind op_kind, BoundExpr *right);
};

struct Binder {
    std::vector<std::string> errors;

    BoundExpr* bind_literal_expr(LiteralExpr *syntax);
    BoundExpr* bind_unary_expr(UnaryExpr *syntax);
    BoundExpr* bind_binary_expr(BinaryExpr *syntax);
    BoundExpr* bind_expr(Expression *syntax);
    BoundUnaryOperatorKind bind_unary_operator_kind(Kind kind, size_t op_type);
    BoundBinaryOperatorKind bind_binary_operator_kind(Kind kind, size_t left_type, size_t right_type);
};

#endif // BINDER_H