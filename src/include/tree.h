#ifndef TREE_H
#define TREE_H

#include "diagnostic.h"
#include "expression.h"

struct Tree {
    DiagnosticBag* diagnostics;
    Expression * root;
    Token eof;

    Tree(DiagnosticBag* diagnostics, Expression *root, Token eof);

    static Tree* parse(std::string text);
};

#endif // TREE_H
