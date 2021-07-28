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
        Tree* tree = Tree::parse(input);
        Compilation* compilation = new Compilation(tree);
        EvaluationResult* result = compilation->evaluate(); 

        if (!result->diagnostics.empty()) {
            for (auto &err : result->diagnostics) {
                printf("%s\n", err.c_str());
            }
        } else {
            std::visit([](auto &&val) {
                if constexpr (std::is_same_v<bool, base_type<decltype(val)>>) {
                    std::cout << (val ? "true" : "false") << std::endl;
                } else if constexpr (std::is_same_v<size, base_type<decltype(val)>>) {
                    std::cout << val << std::endl;
                }
            },
            result->value);
        }
    }
    return 0;
}
