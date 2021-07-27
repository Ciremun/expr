#ifndef BINDER_H
#define BINDER_H

#include <string>
#include <vector>

#include "expression.h"
#include "typedef.h"
#include "util.h"

struct BoundNode {
    BoundNodeKind kind;
};

struct BoundExpr : BoundNode {
    size_t type;

    BoundExpr(size_t type);
    virtual ~BoundExpr() = default;
};

struct BoundLiteralExpr : BoundExpr {
    Value         value;
    BoundNodeKind kind = BoundNodeKind::literal_expr;

    BoundLiteralExpr(Value value, size_t type);
};

struct BoundUnaryOperator {
    Kind syntax_kind;
    BoundUnaryOperatorKind kind;
    size_t operand_type;
    size_t result_type;
    static std::vector<BoundUnaryOperator*> operators;

    BoundUnaryOperator(Kind syntax_kind, BoundUnaryOperatorKind kind, size_t operand_type);
    BoundUnaryOperator(Kind syntax_kind, BoundUnaryOperatorKind kind, size_t operand_type, size_t result_type);
    
    static BoundUnaryOperator* Bind(Kind syntax_kind, size_t operand_type);
};

struct BoundBinaryOperator {
    Kind syntax_kind;
    BoundBinaryOperatorKind kind;
    size_t left_type;
    size_t right_type;
    size_t result_type;
    static std::vector<BoundBinaryOperator*> operators;

    BoundBinaryOperator(Kind syntax_kind, BoundBinaryOperatorKind kind, size_t type);
    BoundBinaryOperator(Kind syntax_kind, BoundBinaryOperatorKind kind, size_t left_type, size_t right_type, size_t result_type);
    
    static BoundBinaryOperator* Bind(Kind syntax_kind, size_t left_type, size_t right_type);
};

struct BoundUnaryExpr : BoundExpr {
    BoundUnaryOperator     op;
    BoundExpr *            operand;
    BoundNodeKind          kind = BoundNodeKind::unary_expr;

    BoundUnaryExpr(BoundUnaryOperator op, BoundExpr *operand);
};

struct BoundBinaryExpr : BoundExpr {
    BoundExpr *             left;
    BoundBinaryOperator     op;
    BoundExpr *             right;
    BoundNodeKind           kind = BoundNodeKind::binary_expr;

    BoundBinaryExpr(BoundExpr *left, BoundBinaryOperator op, BoundExpr *right);
};


struct Binder {
    std::vector<std::string> errors;

    BoundExpr *             bind_literal_expr(LiteralExpr *syntax);
    BoundExpr *             bind_unary_expr(UnaryExpr *syntax);
    BoundExpr *             bind_binary_expr(BinaryExpr *syntax);
    BoundExpr *             bind_expr(Expression *syntax);
};

#endif // BINDER_H
