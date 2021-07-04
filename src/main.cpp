#include <iostream>
#include <string>
#include <iterator>

#include "tree.h"
#include "eval.h"

int main()
{
    while (std::cin) {
        printf("> ");
        std::string input;
        std::getline(std::cin, input);
        if (input.empty()) {
            continue;
        }
        Tree tree = tree.parse(input);
        Binder binder;
        BoundExpr *bound_expr = binder.bind_expr(tree.root);

        tree.errors.insert(
            tree.errors.end(),
            std::make_move_iterator(binder.errors.begin()),
            std::make_move_iterator(binder.errors.end()));

        if (!tree.errors.empty()) {
            for (auto &err : tree.errors) {
                printf("%s\n", err.c_str());
            }
        } else {
            Eval eval(bound_expr);
            size result = eval.evaluate();
            printf("%lld\n", result);
        }
    }
    return 0;
}
