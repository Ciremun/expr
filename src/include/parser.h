#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

#include "token.h"
#include "expression.h"
#include "tree.h"
#include "kind.h"
#include "typedef.h"

struct Parser {
    struct Facts {
        static int binary_operator_precedence(Kind kind);
    };

    std::vector<Token> tokens;
    std::vector<std::string> errors;
    size position = 0;

    Parser(std::string text);
    Token peek(size offset);
    Token current();
    Token next_token();
    Token match_token(Kind kind);
    Expression* parse_expression(int parent_precedence = 0);
    Expression* parse_primary_expression();
    Tree parse();
};

#endif // PARSER_H