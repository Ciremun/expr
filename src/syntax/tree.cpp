#include "tree.h"
#include "parse.h"

Tree::Tree(DiagnosticBag* diagnostics, Expression *root, Token eof)
    : diagnostics(diagnostics), root(root), eof(eof) {}

Tree* Tree::parse(std::string text)
{
    Parser parser(text);
    return parser.parse();
}
