#include "evaluator.h"
#include "util.h"

Eval::Eval(BoundExpr *root, Vars *variables)
    : root(root), variables(variables) {}

Value Eval::evaluate()
{
    return evaluate_expr(root);
}

Value Eval::evaluate_expr(BoundExpr *expr)
{
    switch (expr->kind) {
    case BoundNodeKind::literal_expr: {
        BoundLiteralExpr *e = static_cast<BoundLiteralExpr*>(expr);
        return e->value;
    }
    case BoundNodeKind::variable_expr: {
        BoundVariableExpression *e = static_cast<BoundVariableExpression*>(expr);
        return variables->at(e->name);
    }
    case BoundNodeKind::assignment_expr: {
        BoundAssignmentExpr *e = static_cast<BoundAssignmentExpr*>(expr);
        Value value = evaluate_expr(e->expr);
        variables->insert_or_assign(e->name, value);
        return nullptr;
    }
    case BoundNodeKind::binary_expr: {
        BoundBinaryExpr *e = static_cast<BoundBinaryExpr*>(expr);
        Value left_val = evaluate_expr(e->left);
        Value right_val = evaluate_expr(e->right);

        switch (e->op->kind) {
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
                [](bool left, bool right)
                {
                    return left == right;
                },
                [](size left, size right)
                {
                    return left == right;
                },
                [](auto, auto)
                {
                    runtime_error("%d: unreachable", __LINE__);
                    return false;
                }
            }, left_val, right_val);
        case BoundBinaryOperatorKind::NotEquals:
            return std::visit(overload{
                [](bool left, bool right)
                {
                    return left != right;
                },
                [](size left, size right)
                {
                    return left != right;
                },
                [](auto, auto)
                {
                    runtime_error("%d: unreachable", __LINE__);
                    return false;
                }
            }, left_val, right_val);
        default:
            runtime_error("unexpected binary operator: %s\n", e->kind);
        }
    }
    case BoundNodeKind::unary_expr: {
        BoundUnaryExpr *e = static_cast<BoundUnaryExpr*>(expr);
        Value value = evaluate_expr(e->operand);

        switch (e->op->kind) {
        case BoundUnaryOperatorKind::Identity:
            return std::get<size>(value);
        case BoundUnaryOperatorKind::Negation:
            return -(std::get<size>(value));
        case BoundUnaryOperatorKind::LogicalNegation:
            return !(std::get<bool>(value));
        default:
            runtime_error("unexpected unary operator: %s\n", e->kind);
        }
    }
    default:
        runtime_error("unexpected expr: %s\n", expr->kind);
    }
}

EvaluationResult::EvaluationResult(DiagnosticBag* diagnostics, Value value)
    : diagnostics(diagnostics), value(value) {}

Compilation::Compilation(Tree *syntax)
    : syntax(syntax) {}

EvaluationResult* Compilation::evaluate(Vars *variables)
{
    Binder* binder = new Binder(variables);
    BoundExpr* bound_expr = binder->bind_expr(syntax->root);

    syntax->diagnostics->content.insert(
        syntax->diagnostics->content.end(),
        std::make_move_iterator(binder->diagnostics->content.begin()),
        std::make_move_iterator(binder->diagnostics->content.end()));

    if (!syntax->diagnostics->content.empty()) {
        return new EvaluationResult(syntax->diagnostics, nullptr);
    }

    Eval* evaluator = new Eval(bound_expr, variables);
    Value value = evaluator->evaluate();
    return new EvaluationResult(syntax->diagnostics, value);
}
