#include <iostream>
#include <iterator>
#include <string>

#include "eval.h"
#include "tree.h"
#include "util.h"

int main()
{
    while (std::cin) {
        printf("> ");
        std::string input;
        std::getline(std::cin, input);
        if (input.empty()) {
            continue;
        }
        Tree       tree = tree.parse(input);
        Binder     binder;
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
            Eval  eval(bound_expr);
            Value result = eval.evaluate();
            std::visit([](auto &&val) {
                if constexpr (std::is_same_v<bool, base_type<decltype(val)>>) {
                    std::cout << (val ? "true" : "false") << std::endl;
                } else if constexpr (std::is_same_v<size, base_type<decltype(val)>>) {
                    std::cout << val << std::endl;
                }
            },
            result);
        }
    }
    return 0;
}
