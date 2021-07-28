#ifndef EVAL_H
#define EVAL_H

#include "binder.h"
#include "expression.h"
#include "typedef.h"
#include "tree.h"

struct Eval {
    BoundExpr *root;

    Eval(BoundExpr *root);
    Value evaluate();
    Value evaluate_expr(BoundExpr *expr);
};

struct EvaluationResult
{
    std::vector<std::string> diagnostics;
    Value value;

    EvaluationResult(std::vector<std::string> diagnostics, Value value);
};

struct Compilation
{   
    Tree *syntax;

    Compilation(Tree *syntax);

    EvaluationResult* evaluate();    
};

#endif // EVAL_H
