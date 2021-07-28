#include "tree.h"

Tree::Tree(const std::vector<std::string> &errors, Expression *root, Token eof)
    : errors(errors), root(root), eof(eof) {}

Tree* Tree::parse(std::string text)
{
    Parser parser(text);
    return parser.parse();
}
