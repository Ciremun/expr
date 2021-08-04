#include "binder.h"
#include "util.h"

BoundExpr::BoundExpr(size_t type, BoundNodeKind kind)
    : type(type), kind(kind) {}

BoundUnaryExpr::BoundUnaryExpr(BoundUnaryOperator *op, BoundExpr *operand)
    : BoundExpr(op->result_type, BoundNodeKind::unary_expr), op(op), operand(operand) {}

BoundLiteralExpr::BoundLiteralExpr()
    : BoundExpr(0, BoundNodeKind::literal_expr), value(nullptr) {}

BoundLiteralExpr::BoundLiteralExpr(Value value, size_t type)
    : BoundExpr(type, BoundNodeKind::literal_expr), value(value) {}

BoundBinaryExpr::BoundBinaryExpr(BoundExpr *left, BoundBinaryOperator *op, BoundExpr *right)
    : BoundExpr(op->result_type, BoundNodeKind::binary_expr), left(left), op(op), right(right) {}

BoundVariableExpression::BoundVariableExpression(std::string name, size_t type)
    : BoundExpr(type, BoundNodeKind::variable_expr), name(name) {}

BoundAssignmentExpr::BoundAssignmentExpr(std::string name, BoundExpr *expr)
    : BoundExpr(expr->type, BoundNodeKind::assignment_expr), name(name), expr(expr) {}

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

Binder::Binder(Vars *variables)
    : variables(variables) {}

BoundExpr *Binder::bind_literal_expr(LiteralExpr *syntax)
{
    return new BoundLiteralExpr(syntax->value, syntax->value.index());
}

BoundExpr *Binder::bind_unary_expr(UnaryExpr *syntax)
{
    BoundExpr* bound_operand = bind_expr(syntax->operand);
    BoundUnaryOperator* bound_operator = BoundUnaryOperator::Bind(syntax->op.kind, bound_operand->type);
    if (bound_operator == nullptr) {
        diagnostics->report_undefined_unary_operator(syntax->op.span, syntax->op.text, bound_operand->type);
        return bound_operand;
    }
    return new BoundUnaryExpr(bound_operator, bound_operand);
}

BoundExpr *Binder::bind_binary_expr(BinaryExpr *syntax)
{
    BoundExpr* bound_left = bind_expr(syntax->left);
    BoundExpr* bound_right = bind_expr(syntax->right);
    BoundBinaryOperator* bound_operator = BoundBinaryOperator::Bind(syntax->op.kind, bound_left->type, bound_right->type);
    if (bound_operator == nullptr) {
        diagnostics->report_undefined_binary_operator(syntax->op.span, syntax->op.text, bound_left->type, bound_right->type);
        return bound_left;
    }
    return new BoundBinaryExpr(bound_left, bound_operator, bound_right);
}

BoundExpr* Binder::bind_paren_expr(ParenExpr *syntax)
{
    return bind_expr(syntax->expr);
}

BoundExpr* Binder::bind_name_expr(NameExpr *syntax)
{
    std::string name = syntax->identifier.text;
    if (!variables->contains(name)) {
        diagnostics->report_undefined_name(syntax->identifier.span, name);
        return new BoundLiteralExpr();
    }
    Value &value = variables->at(name);
    size_t type = value.index();
    return new BoundVariableExpression(name, type);
}

BoundExpr* Binder::bind_assignment_expr(AssignmentExpr *syntax)
{
    std::string name = syntax->identifier.text;
    BoundExpr *bound_expr = bind_expr(syntax->expr);
    Value default_value =
        bound_expr->type == variant_index<Value, size>()
        ? Value(static_cast<size>(0))
        :
        bound_expr->type == variant_index<Value, bool>()
        ? Value(false)
        : Value(nullptr);
    if (std::holds_alternative<std::nullptr_t>(default_value))
        runtime_error("unsupported variable type: %s\n", variant_types[bound_expr->type]);
    // variables->insert({name, default_value});
    return new BoundAssignmentExpr(name, bound_expr);
}

BoundExpr *Binder::bind_expr(Expression *syntax)
{
    switch (syntax->kind) {
    case Kind::literal_expr:
        return bind_literal_expr(static_cast<LiteralExpr*>(syntax));
    case Kind::unary_expr:
        return bind_unary_expr(static_cast<UnaryExpr*>(syntax));
    case Kind::binary_expr:
        return bind_binary_expr(static_cast<BinaryExpr*>(syntax));
    case Kind::paren_expr:
        return bind_paren_expr(static_cast<ParenExpr*>(syntax));
    case Kind::name_expr:
        return bind_name_expr(static_cast<NameExpr*>(syntax));
    case Kind::assignment_expr:
        return bind_assignment_expr(static_cast<AssignmentExpr*>(syntax));
    default:
        runtime_error("Unexpected syntax <%s>\n", kinds[syntax->kind]);
    }
}
