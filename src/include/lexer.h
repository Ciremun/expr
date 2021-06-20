#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

#include "token.h"
#include "typedef.h"

struct Lexer
{
    std::string text;
    size position = 0;
    std::vector<std::string> errors;

    Lexer(std::string text);
    char current_char();
    void next_char();
    Token lex();
};

#endif // LEXER_H