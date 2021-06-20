#include <iostream>
#include <string>

#include "tree.h"
#include "eval.h"

int main()
{
    while (std::cin)
    {
        printf("> ");
        std::string input;
        std::getline(std::cin, input);
        if (input.empty())
        {
            continue;
        }
        Tree tree = tree.parse(input);
        if (!tree.errors.empty())
        {
            for (auto &err : tree.errors)
            {
                printf("%s\n", err.c_str());
            }
        }
        else
        {
            Eval eval(tree.root);
            size result = eval.evaluate();
            printf("%lld\n", result);
        }
    }
    return 0;
}
