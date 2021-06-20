#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

#include "token.h"
#include "expression.h"
#include "tree.h"
#include "kind.h"
#include "typedef.h"

struct Parser
{
    std::vector<Token> tokens;
    std::vector<std::string> errors;
    size position = 0;

    Parser(std::string text);
    Token peek(size offset);
    Token current();
    Token next_token();
    Token match_token(Kind kind);
    Expression* parse_expr();
    Expression* parse_primary();
    Expression* parse_term();
    Expression* parse_factor();
    Tree parse();
};

#endif // PARSER_H