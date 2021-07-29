#include <iostream>
#include <string>

#include "evaluator.h"
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

        if (!result->diagnostics->content.empty()) {
            for (auto &err : result->diagnostics->content) {
                printf("[ERROR] %s\n", err->message.c_str());
            }
        } else {
            std::visit(overload {
                [](bool val)
                {
                    printf("%s\n", val ? "true" : "false");
                },
                [](size val)
                {
                    printf("%lld\n", val);
                },
                [](auto)
                {
                    runtime_error("unreachable");
                }
            },
            result->value);
        }
    }
    return 0;
}
