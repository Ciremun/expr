#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <vector>
#include <string>
#include <unordered_map>

#include "binder.h"
#include "typedef.h"
#include "tree.h"
#include "diagnostic.h"

struct Eval {
    BoundExpr *root;
    Vars *variables;

    Eval(BoundExpr *root, Vars *variables);

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

    EvaluationResult* evaluate(Vars *variables);
};

#endif // EVALUATOR_H
