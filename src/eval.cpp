#include "eval.h"

Eval::Eval(Expression* root) : root(root) {}

size Eval::evaluate()
{
    return evaluate_expr(root);
}

size Eval::evaluate_expr(Expression* expr)
{
    if (LiteralExpr* literal_expr = dynamic_cast<LiteralExpr*>(expr))
    {
        return std::get<size>(literal_expr->literal.value);
    }
    if (BinaryExpr* binary_expr = dynamic_cast<BinaryExpr*>(expr))
    {
        size left = evaluate_expr(binary_expr->left);
        size right = evaluate_expr(binary_expr->right);

        if (binary_expr->op.kind == Kind::plus_token)          return left + right;
        if (binary_expr->op.kind == Kind::minus_token)         return left - right;
        if (binary_expr->op.kind == Kind::star_token)          return left * right;
        if (binary_expr->op.kind == Kind::forward_slash_token) return left / right;

        runtime_error("unexpected binary operator: %s\n", kinds[binary_expr->kind]);
    }
    if (ParenExpr* paren_expr = dynamic_cast<ParenExpr*>(expr))
    {
        return evaluate_expr(paren_expr->expr);
    }
    runtime_error("unexpected expr: %s\n", kinds[expr->kind]);
}