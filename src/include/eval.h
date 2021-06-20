#ifndef EVAL_H
#define EVAL_H

#include "expression.h"
#include "typedef.h"

struct Eval {
    Expression* root;

    Eval(Expression* root);
    size evaluate();
    size evaluate_expr(Expression* expr);
};

#endif // EVAL_H