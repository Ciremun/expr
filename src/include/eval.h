#ifndef EVAL_H
#define EVAL_H

#include "expression.h"

struct Eval
{
    Expression* root;

    Eval(Expression* root);
    size_t evaluate();
    size_t evaluate_expr(Expression* expr);
};

#endif // EVAL_H