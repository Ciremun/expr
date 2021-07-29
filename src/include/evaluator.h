#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <vector>

#include "binder.h"
#include "typedef.h"
#include "tree.h"
#include "diagnostic.h"

struct Eval {
    BoundExpr *root;

    Eval(BoundExpr *root);
    Value evaluate();
    Value evaluate_expr(BoundExpr *expr);
};

struct EvaluationResult {
    DiagnosticBag* diagnostics;
    Value value;

    EvaluationResult(DiagnosticBag* diagnostics, Value value);
};

struct Compilation {
    Tree *syntax;

    Compilation(Tree *syntax);

    EvaluationResult* evaluate();
};

#endif // EVALUATOR_H
