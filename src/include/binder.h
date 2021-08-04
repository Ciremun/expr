#ifndef BINDER_H
#define BINDER_H

#include <vector>
#include <string>

#include "typedef.h"
#include "kind.h"
#include "diagnostic.h"
#include "expression.h"

struct BoundExpr {
    size_t type;
    BoundNodeKind kind;

    BoundExpr(size_t type, BoundNodeKind kind);
    virtual ~BoundExpr() = default;
};

struct BoundLiteralExpr : BoundExpr {
    Value value;

    BoundLiteralExpr();
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
    BoundBinaryOperator(Kind syntax_kind, BoundBinaryOperatorKind kind, size_t operand_type, size_t result_type);
    BoundBinaryOperator(Kind syntax_kind, BoundBinaryOperatorKind kind, size_t left_type, size_t right_type, size_t result_type);

    static BoundBinaryOperator* Bind(Kind syntax_kind, size_t left_type, size_t right_type);
};

struct BoundUnaryExpr : BoundExpr {
    BoundUnaryOperator *op;
    BoundExpr *operand;

    BoundUnaryExpr(BoundUnaryOperator *op, BoundExpr *operand);
};

struct BoundBinaryExpr : BoundExpr {
    BoundExpr *left;
    BoundBinaryOperator*op;
    BoundExpr *right;

    BoundBinaryExpr(BoundExpr *left, BoundBinaryOperator *op, BoundExpr *right);
};

struct BoundVariableExpression : BoundExpr {
    std::string name;

    BoundVariableExpression(std::string name, size_t type);
};

struct BoundAssignmentExpr : BoundExpr {
    std::string name;
    BoundExpr *expr;

    BoundAssignmentExpr(std::string name, BoundExpr *expr);
};

struct Binder {
    DiagnosticBag* diagnostics = new DiagnosticBag();
    Vars *variables;

    Binder(Vars *variables);

    BoundExpr* bind_literal_expr(LiteralExpr *syntax);
    BoundExpr* bind_unary_expr(UnaryExpr *syntax);
    BoundExpr* bind_binary_expr(BinaryExpr *syntax);
    BoundExpr* bind_paren_expr(ParenExpr *syntax);
    BoundExpr* bind_name_expr(NameExpr *syntax);
    BoundExpr* bind_assignment_expr(AssignmentExpr *syntax);
    BoundExpr* bind_expr(Expression *syntax);
};

#endif // BINDER_H
