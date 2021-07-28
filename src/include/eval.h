#ifndef EVAL_H
#define EVAL_H

#include "binder.h"
#include "expression.h"
#include "typedef.h"

struct Eval {
    BoundExpr *root;

    Eval(BoundExpr *root);
    Value evaluate();
    Value evaluate_expr(BoundExpr *expr);
};

#endif // EVAL_H