#include <iostream>
#include <string>
#include <unordered_map>

#include "evaluator.h"
#include "tree.h"
#include "util.h"
#include "typedef.h"

int main()
{
    auto variables = new std::unordered_map<std::string, Value>();
    while (std::cin) {
        printf("> ");
        std::string input;
        std::getline(std::cin, input);
        if (input.empty()) {
            continue;
        }
        Tree* tree = Tree::parse(input);
        Compilation* compilation = new Compilation(tree);
        EvaluationResult* result = compilation->evaluate(variables);

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
                [](std::nullptr_t) {},
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
