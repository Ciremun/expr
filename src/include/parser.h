#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "diagnostic.h"
#include "typedef.h"
#include "token.h"
#include "tree.h"
#include "expression.h"

struct Parser {
    std::vector<Token> tokens;
    DiagnosticBag* diagnostics = new DiagnosticBag();
    size position = 0;

    Parser(std::string text);
    Token peek(size offset);
    Token current();
    Token next_token();
    Token match_token(Kind kind);
    Expression *parse_expression();
    Expression *parse_primary_expression();
    Expression *parse_assignment_expression();
    Expression *parse_binary_expression(int parent_precedence = 0);
    Tree* parse();

    struct Facts {
        static int binary_operator_precedence(Kind kind);
        static int unary_operator_precedence(Kind kind);
        static Kind keyword_kind(std::string &text);
    };
};

#endif // PARSER_H
