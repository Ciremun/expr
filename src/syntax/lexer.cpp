#include "lexer.h"
#include "parser.h"
#include "util.h"

Lexer::Lexer(std::string text)
    : text(text), position(0) {}

char Lexer::peek(int offset)
{
    usize index = position + offset;
    if (index >= text.length())
        return '\0';
    return text[index];
}

char Lexer::current_char()
{
    return peek(0);
}

char Lexer::lookahead()
{
    return peek(1);
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

    if (is_digit(current)) {
        size start = position;
        do {
            next_char();
        } while (is_digit(current_char()));
        size length = position - start;
        std::string text = this->text.substr(start, length);
        usize value = 0;
        if (!string_to_size(text, &value))
            this->diagnostics->report_invalid_number(new TextSpan(start, length), this->text, variant_index<Value, size>());
        return Token(Kind::number_token, start, text, static_cast<size>(value));
    }

    if (current == ' ') {
        size start = position;
        do {
            next_char();
        } while (current_char() == ' ');
        size length = position - start;
        std::string text = this->text.substr(start, length);
        return Token(Kind::space_token, start, text, nullptr);
    }

    if (is_letter(current)) {
        size start = position;
        do {
            next_char();
        } while (is_letter(current_char()));
        size length = position - start;
        std::string text = this->text.substr(start, length);
        Kind kind = Parser::Facts::keyword_kind(text);
        return Token(kind, start, text, nullptr);
    }

    switch (current) {
    case '+':
        return Token(Kind::plus_token, position++, "+", nullptr);
    case '-':
        return Token(Kind::minus_token, position++, "-", nullptr);
    case '*':
        return Token(Kind::star_token, position++, "*", nullptr);
    case '/':
        return Token(Kind::forward_slash_token, position++, "/", nullptr);
    case '(':
        return Token(Kind::open_paren_token, position++, "(", nullptr);
    case ')':
        return Token(Kind::close_paren_token, position++, ")", nullptr);
    case '&': {
        if (lookahead() == '&')
            return Token(Kind::double_ampersand_token, position += 2, "&&", nullptr);
        break;
    }
    case '|': {
        if (lookahead() == '|')
            return Token(Kind::double_pipe_token, position += 2, "||", nullptr);
        break;
    }
    case '=': {
        if (lookahead() == '=')
            return Token(Kind::double_equals_token, position += 2, "==", nullptr);
        break;
    }
    case '!': {
        if (lookahead() == '=')
            return Token(Kind::bang_equals_token, position += 2, "!=", nullptr);
        return Token(Kind::bang_token, position++, "!", nullptr);
        break;
    }
    }

    diagnostics->report_bad_char(position, current);

    int temp = position;
    position++;

    return Token(Kind::error_token, temp, text.substr(temp, 1), nullptr);
}
