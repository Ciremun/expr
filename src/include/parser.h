#ifndef PARSER_H
#define PARSER_H

#include "diagnostic.h"

struct Parser {
    std::vector<Token> tokens;
    DiagnosticBag* diagnostics = new DiagnosticBag();
    size position = 0;

    Parser(std::string text);
    Token peek(size offset);
    Token current();
    Token next_token();
    Token match_token(Kind kind);
    Expression *parse_expression(int parent_precedence = 0);
    Expression *parse_primary_expression();
    Tree* parse();

    struct Facts {
        static int binary_operator_precedence(Kind kind);
        static int unary_operator_precedence(Kind kind);
        static Kind keyword_kind(std::string &text);
    };
};

#endif // PARSER_H
