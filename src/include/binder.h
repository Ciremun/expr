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
    Value type;

    virtual ~BoundExpr() = default;
};

struct BoundLiteralExpr : BoundExpr {
    Value value;
    BoundNodeKind kind = BoundNodeKind::literal_expr;
    Value type = BoundNodeKind::literal_expr;

    BoundLiteralExpr(Value value);
};

struct BoundUnaryExpr : BoundExpr {
    BoundUnaryOperatorKind op_kind;
    BoundExpr *operand;
    BoundNodeKind kind = BoundNodeKind::unary_expr;
    Value type;

    BoundUnaryExpr(BoundUnaryOperatorKind op_kind, BoundExpr *operand);
};

struct BoundBinaryExpr : BoundExpr {
    BoundExpr *left;
    BoundBinaryOperatorKind op_kind;
    BoundExpr *right;
    BoundNodeKind kind = BoundNodeKind::binary_expr;
    Value type;

    BoundBinaryExpr(BoundExpr *left, BoundBinaryOperatorKind op_kind, BoundExpr *right);
};

struct Binder {
    std::vector<std::string> errors;

    BoundExpr* bind_literal_expr(LiteralExpr *syntax);
    BoundExpr* bind_unary_expr(UnaryExpr *syntax);
    BoundExpr* bind_binary_expr(BinaryExpr *syntax);
    BoundExpr* bind_expr(Expression *syntax);
    BoundUnaryOperatorKind bind_unary_operator_kind(Kind kind, Value op_type);
    BoundBinaryOperatorKind bind_binary_operator_kind(Kind kind, Value left_type, Value right_type);
};

#endif // BINDER_H