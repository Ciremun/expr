#include "eval.h"

Eval::Eval(BoundExpr* root)
    : root(root)
{
}

Value Eval::evaluate()
{
    return evaluate_expr(root);
}

Value Eval::evaluate_expr(BoundExpr* expr)
{
    if (BoundLiteralExpr* literal_expr = dynamic_cast<BoundLiteralExpr*>(expr)) {
        return literal_expr->value;
    }
    if (BoundBinaryExpr* binary_expr = dynamic_cast<BoundBinaryExpr*>(expr)) {
        Value left_val = evaluate_expr(binary_expr->left);
        Value right_val = evaluate_expr(binary_expr->right);

        size left = std::get<size>(left_val);
        size right = std::get<size>(right_val);

        if (binary_expr->op_kind == BoundBinaryOperatorKind::Addition      )  return left + right;
        if (binary_expr->op_kind == BoundBinaryOperatorKind::Subtraction   )  return left - right;
        if (binary_expr->op_kind == BoundBinaryOperatorKind::Multiplication)  return left * right;
        if (binary_expr->op_kind == BoundBinaryOperatorKind::Division      )  return left / right;

        runtime_error("unexpected binary operator: %s\n", binary_expr->kind);
    }
    if (BoundUnaryExpr* unary_expr = dynamic_cast<BoundUnaryExpr*>(expr)) {
        Value value = evaluate_expr(unary_expr->operand);
        size op = std::get<size>(value);

        if (unary_expr->op_kind == BoundUnaryOperatorKind::Identity)  return  op;
        if (unary_expr->op_kind == BoundUnaryOperatorKind::Negation)  return -op;

        runtime_error("unexpected unary operator: %s\n", unary_expr->kind);
    }
    runtime_error("unexpected expr: %s\n", expr->kind);
}