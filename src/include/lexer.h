#ifndef LEXER_H
#define LEXER_H

#include "diagnostic.h"

struct Lexer {
    std::string text;
    usize position = 0;
    DiagnosticBag* diagnostics = new DiagnosticBag();

    Lexer(std::string text);
    char peek(int offset);
    char current_char();
    char lookahead();
    void next_char();
    Token lex();
};

#endif // LEXER_H
