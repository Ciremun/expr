#include "binder.h"
#include "kind.h"
#include "util.h"

BoundExpr::BoundExpr(size_t type)
    : type(type) {}

BoundUnaryExpr::BoundUnaryExpr(BoundUnaryOperator *op, BoundExpr *operand)
    : BoundExpr(op->result_type), op(op), operand(operand) {}

BoundLiteralExpr::BoundLiteralExpr(Value value, size_t type)
    : BoundExpr(type), value(value) {}

BoundBinaryExpr::BoundBinaryExpr(BoundExpr *left, BoundBinaryOperator *op, BoundExpr *right)
    : BoundExpr(op->result_type), left(left), op(op), right(right) {}

BoundUnaryOperator::BoundUnaryOperator(Kind syntax_kind, BoundUnaryOperatorKind kind, size_t operand_type)
    : syntax_kind(syntax_kind), kind(kind), operand_type(operand_type), result_type(operand_type) {}

BoundUnaryOperator::BoundUnaryOperator(Kind syntax_kind, BoundUnaryOperatorKind kind, size_t operand_type, size_t result_type)
    : syntax_kind(syntax_kind), kind(kind), operand_type(operand_type), result_type(result_type) {}

std::vector<BoundUnaryOperator*> BoundUnaryOperator::operators = [] {
    std::vector<BoundUnaryOperator*> v = {
        new BoundUnaryOperator(Kind::bang_token, BoundUnaryOperatorKind::LogicalNegation, variant_index<Value, bool>()),
        new BoundUnaryOperator(Kind::plus_token, BoundUnaryOperatorKind::Identity, variant_index<Value, size>()),
        new BoundUnaryOperator(Kind::minus_token, BoundUnaryOperatorKind::Negation, variant_index<Value, size>())
    };
    return v;
}();

BoundUnaryOperator* BoundUnaryOperator::Bind(Kind syntax_kind, size_t operand_type)
{
    for (const auto &op : BoundUnaryOperator::operators) {
        if (op->syntax_kind == syntax_kind && op->operand_type == operand_type)
            return op;
    }
    return nullptr;
}

BoundBinaryOperator::BoundBinaryOperator(Kind syntax_kind, BoundBinaryOperatorKind kind, size_t type)
    : syntax_kind(syntax_kind), kind(kind), left_type(type), right_type(type), result_type(type) {}

BoundBinaryOperator::BoundBinaryOperator(Kind syntax_kind, BoundBinaryOperatorKind kind, size_t operand_type, size_t result_type)
    : syntax_kind(syntax_kind), kind(kind), left_type(operand_type), right_type(operand_type), result_type(result_type) {}

BoundBinaryOperator::BoundBinaryOperator(Kind syntax_kind, BoundBinaryOperatorKind kind, size_t left_type, size_t right_type, size_t result_type)
    : syntax_kind(syntax_kind), kind(kind), left_type(left_type), right_type(right_type), result_type(result_type) {}

std::vector<BoundBinaryOperator*> BoundBinaryOperator::operators = [] {
    std::vector<BoundBinaryOperator*> v = {
        new BoundBinaryOperator(Kind::plus_token, BoundBinaryOperatorKind::Addition, variant_index<Value, size>()),
        new BoundBinaryOperator(Kind::minus_token, BoundBinaryOperatorKind::Subtraction, variant_index<Value, size>()),
        new BoundBinaryOperator(Kind::star_token, BoundBinaryOperatorKind::Multiplication, variant_index<Value, size>()),
        new BoundBinaryOperator(Kind::forward_slash_token, BoundBinaryOperatorKind::Division, variant_index<Value, size>()),
        new BoundBinaryOperator(Kind::double_equals_token, BoundBinaryOperatorKind::Equals, variant_index<Value, size>(), variant_index<Value, bool>()),
        new BoundBinaryOperator(Kind::bang_equals_token, BoundBinaryOperatorKind::NotEquals, variant_index<Value, size>(), variant_index<Value, bool>()),
        new BoundBinaryOperator(Kind::double_ampersand_token, BoundBinaryOperatorKind::LogicalAnd, variant_index<Value, bool>()),
        new BoundBinaryOperator(Kind::double_equals_token, BoundBinaryOperatorKind::Equals, variant_index<Value, bool>()),
        new BoundBinaryOperator(Kind::bang_equals_token, BoundBinaryOperatorKind::NotEquals, variant_index<Value, bool>()),
        new BoundBinaryOperator(Kind::double_pipe_token, BoundBinaryOperatorKind::LogicalOr, variant_index<Value, bool>()),
    };
    return v;
}();

BoundBinaryOperator* BoundBinaryOperator::Bind(Kind syntax_kind, size_t left_type, size_t right_type)
{
    for (const auto &op : BoundBinaryOperator::operators) {
        if (op->syntax_kind == syntax_kind && op->left_type == left_type && op->right_type == right_type)
            return op;
    }
    return nullptr;
}

BoundExpr *Binder::bind_literal_expr(LiteralExpr *syntax)
{
    return new BoundLiteralExpr(syntax->value, syntax->value.index());
}

BoundExpr *Binder::bind_unary_expr(UnaryExpr *syntax)
{
    BoundExpr* bound_operand = bind_expr(syntax->operand);
    BoundUnaryOperator* bound_operator = BoundUnaryOperator::Bind(syntax->op.kind, bound_operand->type);
    if (bound_operator == nullptr) {
        errors.push_back(format("Unary operator <%s> is not defined for type <%s>", syntax->op.text.c_str(), variant_types[bound_operand->type]));
        return bound_operand;
    }
    return new BoundUnaryExpr(bound_operator, bound_operand);
}

BoundExpr *Binder::bind_binary_expr(BinaryExpr *syntax)
{
    BoundExpr*             bound_left = bind_expr(syntax->left);
    BoundExpr*             bound_right = bind_expr(syntax->right);
    BoundBinaryOperator* bound_operator = BoundBinaryOperator::Bind(syntax->op.kind, bound_left->type, bound_right->type);
    if (bound_operator == nullptr) {
        errors.push_back(format("Binary operator <%s> is not defined for types <%s> and <%s>", syntax->op.text.c_str(), variant_types[bound_left->type], variant_types[bound_right->type]));
        return bound_left;
    }
    return new BoundBinaryExpr(bound_left, bound_operator, bound_right);
}

BoundExpr *Binder::bind_expr(Expression *syntax)
{
    if (LiteralExpr *literal_expr = dynamic_cast<LiteralExpr *>(syntax))
        return bind_literal_expr(literal_expr);
    if (UnaryExpr *unary_expr = dynamic_cast<UnaryExpr *>(syntax))
        return bind_unary_expr(unary_expr);
    if (BinaryExpr *binary_expr = dynamic_cast<BinaryExpr *>(syntax))
        return bind_binary_expr(binary_expr);
    if (ParenExpr *paren_expr = dynamic_cast<ParenExpr *>(syntax))
        return bind_expr(paren_expr->expr);
    runtime_error("Unexpected syntax <%s>\n", kinds[syntax->kind]);
}
