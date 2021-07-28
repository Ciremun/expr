#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "expression.h"
#include "kind.h"
#include "token.h"
#include "tree.h"
#include "typedef.h"

struct Facts {
    static int  binary_operator_precedence(Kind kind);
    static int  unary_operator_precedence(Kind kind);
    static Kind keyword_kind(std::string &text);
};

struct Parser {
    std::vector<Token>       tokens;
    std::vector<std::string> errors;
    size                     position = 0;

    Parser(std::string text);
    Token       peek(size offset);
    Token       current();
    Token       next_token();
    Token       match_token(Kind kind);
    Expression *parse_expression(int parent_precedence = 0);
    Expression *parse_primary_expression();
    Tree*       parse();
};

#endif // PARSER_H
