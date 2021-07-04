#ifndef EVAL_H
#define EVAL_H

#include "expression.h"
#include "typedef.h"
#include "binder.h"

struct Eval {
    BoundExpr* root;

    Eval(BoundExpr* root);
    size evaluate();
    size evaluate_expr(BoundExpr* expr);
};

#endif // EVAL_H