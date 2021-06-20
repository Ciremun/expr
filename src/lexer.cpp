#include "lexer.h"
#include "kind.h"
#include "util.h"

Lexer::Lexer(std::string text)
    : text(text), position(0)
{
}

char Lexer::current_char()
{
    if (position >= text.length())
        return '\0';
    return text[position];
}

void Lexer::next_char()
{
    position++;
}

Token Lexer::lex()
{
    if (position >= text.length())
        return Token(Kind::eof_token, position, "\0", nullptr);

    char current = current_char();

    if (is_digit(current))
    {
        size start = position;
        next_char();
        while (is_digit(current_char()))
            next_char();
        size length = position - start;
        std::string text = this->text.substr(start, length);
        size value = 0;
        if (!string_to_size(text, &value))
            errors.push_back(format("[ERROR] the number '%s' isn't valid size", text.c_str()));
        return Token(Kind::number_token, start, text, value);
    }

    if (current == ' ')
    {
        size start = position;
        next_char();
        while (current_char() == ' ')
            next_char();
        size length = position - start;
        std::string text = this->text.substr(start, length);
        return Token(Kind::space_token, start, text, nullptr);
    }

    int temp = position;
    position++;

    switch (current)
    {
        case '+': return Token(Kind::plus_token,          temp, "+", nullptr);
        case '-': return Token(Kind::minus_token,         temp, "-", nullptr);
        case '*': return Token(Kind::star_token,          temp, "*", nullptr);
        case '/': return Token(Kind::forward_slash_token, temp, "/", nullptr);
        case '(': return Token(Kind::open_paren_token,    temp, "(", nullptr);
        case ')': return Token(Kind::close_paren_token,   temp, ")", nullptr);
    }

    errors.push_back(format("[ERROR] bad input char: '%c'", current));
    return Token(Kind::error_token, temp, text.substr(temp, 1), nullptr);
}