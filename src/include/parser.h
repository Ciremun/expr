#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

#include "token.h"
#include "expression.h"
#include "tree.h"
#include "kind.h"

struct Parser
{
    std::vector<Token> tokens;
    std::vector<std::string> errors;
    size_t position = 0;

    Parser(std::string text);
    Token peek(size_t offset);
    Token current();
    Token next_token();
    Token match(Kind kind);
    Expression* parse_expr();
    Expression* parse_primary();
    Tree parse();
    Expression* parse_term();
    Expression* parse_factor();
};

#endif // PARSER_H