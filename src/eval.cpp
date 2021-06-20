#include "eval.h"

Eval::Eval(Expression* root) : root(root) {}

size Eval::evaluate()
{
    return evaluate_expr(root);
}

size Eval::evaluate_expr(Expression* expr)
{
    if (NumberExpr* number_expr = dynamic_cast<NumberExpr*>(expr))
    {
        return std::get<size>(number_expr->number.value);
    }
    if (BinaryExpr* binary_expr = dynamic_cast<BinaryExpr*>(expr))
    {
        size left = evaluate_expr(binary_expr->left);
        size right = evaluate_expr(binary_expr->right);

        if (binary_expr->op.kind == Kind::plus)          return left + right;
        if (binary_expr->op.kind == Kind::minus)         return left - right;
        if (binary_expr->op.kind == Kind::star)          return left * right;
        if (binary_expr->op.kind == Kind::forward_slash) return left / right;

        runtime_error("unexpected binary operator: %s\n", kinds[binary_expr->kind]);
    }
    if (ParenExpr* paren_expr = dynamic_cast<ParenExpr*>(expr))
    {
        return evaluate_expr(paren_expr->expr);
    }
    runtime_error("unexpected expr: %s\n", kinds[expr->kind]);
}