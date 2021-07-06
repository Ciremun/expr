#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>

#include "expression.h"
#include "token.h"

struct Tree {
    std::vector<std::string> errors;
    Expression *             root;
    Token                    eof;

    Tree(const std::vector<std::string> &errors, Expression *root, Token eof);

    Tree parse(std::string text);
};

#endif // TREE_H