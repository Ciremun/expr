#include "eval.h"

Eval::Eval(BoundExpr *root)
    : root(root) {}

Value Eval::evaluate()
{
    return evaluate_expr(root);
}

Value Eval::evaluate_expr(BoundExpr *expr)
{
    if (BoundLiteralExpr *literal_expr = dynamic_cast<BoundLiteralExpr *>(expr)) {
        return literal_expr->value;
    }
    if (BoundBinaryExpr *binary_expr = dynamic_cast<BoundBinaryExpr *>(expr)) {
        Value left_val = evaluate_expr(binary_expr->left);
        Value right_val = evaluate_expr(binary_expr->right);

        switch (binary_expr->op.kind) {
        case BoundBinaryOperatorKind::Addition:
            return std::get<size>(left_val) + std::get<size>(right_val);
        case BoundBinaryOperatorKind::Subtraction:
            return std::get<size>(left_val) - std::get<size>(right_val);
        case BoundBinaryOperatorKind::Multiplication:
            return std::get<size>(left_val) * std::get<size>(right_val);
        case BoundBinaryOperatorKind::Division:
            return std::get<size>(left_val) / std::get<size>(right_val);
        case BoundBinaryOperatorKind::LogicalAnd:
            return std::get<bool>(left_val) && std::get<bool>(right_val);
        case BoundBinaryOperatorKind::LogicalOr:
            return std::get<bool>(left_val) || std::get<bool>(right_val);
        case BoundBinaryOperatorKind::Equals:
            return std::visit(overload{
                        [](bool left, bool right) { return left == right; },
                        [](size left, size right) { return left == right; },
                        [](auto, auto) { runtime_error("%d: unreachable", __LINE__); return false; }
                    }, left_val, right_val);
        case BoundBinaryOperatorKind::NotEquals:
            return std::visit(overload{
                        [](bool left, bool right) { return left != right; },
                        [](size left, size right) { return left != right; },
                        [](auto, auto) { runtime_error("%d: unreachable", __LINE__); return false; }
                    }, left_val, right_val);
        default:
            runtime_error("unexpected binary operator: %s\n", binary_expr->kind);
        }
    }
    if (BoundUnaryExpr *unary_expr = dynamic_cast<BoundUnaryExpr *>(expr)) {
        Value value = evaluate_expr(unary_expr->operand);

        switch (unary_expr->op.kind) {
        case BoundUnaryOperatorKind::Identity:
            return std::get<size>(value);
        case BoundUnaryOperatorKind::Negation:
            return -(std::get<size>(value));
        case BoundUnaryOperatorKind::LogicalNegation:
            return !(std::get<bool>(value));
        default:
            runtime_error("unexpected unary operator: %s\n", unary_expr->kind);
        }
    }
    runtime_error("unexpected expr: %s\n", expr->kind);
}
