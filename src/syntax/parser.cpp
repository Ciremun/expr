#include "parser.h"
#include "lexer.h"

Parser::Parser(std::string text)
{
    std::vector<Token> tokens;
    Lexer lexer(text);
    Token token;
    do {
        token = lexer.lex();
        if (token.kind != Kind::space_token && token.kind != Kind::error_token) {
            tokens.push_back(token);
        }
    } while (token.kind != Kind::eof_token);
    this->tokens = tokens;
    diagnostics->content.insert(
        diagnostics->content.end(),
        std::make_move_iterator(lexer.diagnostics->content.begin()),
        std::make_move_iterator(lexer.diagnostics->content.end()));
}

Token Parser::peek(size offset)
{
    usize index = position + offset;
    if (index >= tokens.size())
        return tokens[tokens.size() - 1];
    return tokens[index];
}

Token Parser::current()
{
    return peek(0);
}

Token Parser::next_token()
{
    Token cur = current();
    position++;
    return cur;
}

Token Parser::match_token(Kind kind)
{
    Token cur = current();
    if (cur.kind == kind) {
        return next_token();
    }
    diagnostics->report_unexpected_token(cur.span, cur.kind, kind);
    return Token(kind, cur.position, "\0", nullptr);
}

Expression *Parser::parse_expression()
{
    return parse_assignment_expression();
}

Expression *Parser::parse_primary_expression()
{
    switch (current().kind) {
    case Kind::open_paren_token: {
        Token left = next_token();
        Expression *expr = parse_expression();
        Token right = match_token(Kind::close_paren_token);
        return new ParenExpr(left, expr, right);
    }
    case Kind::true_keyword:
    case Kind::false_keyword: {
        Token current_token = current();
        Token keyword_token = next_token();
        bool value = keyword_token.kind == Kind::true_keyword;
        return new LiteralExpr(keyword_token, value);
    }
    case Kind::identifier_token: {
        Token identifier = next_token();
        return new NameExpr(identifier);
    }
    default: {
        Token number = match_token(Kind::number_token);
        return new LiteralExpr(number);
    }
    }
}

Expression *Parser::parse_assignment_expression()
{
    if (peek(0).kind == Kind::identifier_token &&
            peek(1).kind == Kind::double_equals_token) {
        Token identifier = next_token();
        Token op = next_token();
        Expression *right = parse_assignment_expression();
        return new AssignmentExpr(identifier, op, right);
    }
    return parse_binary_expression();
}

Expression *Parser::parse_binary_expression(int parent_precedence)
{
    Expression *left;
    int unary_operator_precedence = Parser::Facts::unary_operator_precedence(current().kind);
    if (unary_operator_precedence != 0 && unary_operator_precedence >= parent_precedence) {
        Token op = next_token();
        Expression *operand = parse_binary_expression(unary_operator_precedence);
        left = new UnaryExpr(op, operand);
    } else {
        left = parse_primary_expression();
    }

    while (1) {
        int precedence = Parser::Facts::binary_operator_precedence(current().kind);
        if (precedence == 0 || precedence <= parent_precedence)
            break;
        Token operator_token = next_token();
        Expression *right = parse_binary_expression(precedence);
        left = new BinaryExpr(left, operator_token, right);
    }

    return left;
}

Tree* Parser::parse()
{
    Expression *expression = parse_expression();
    Token eof = match_token(Kind::eof_token);
    return new Tree(diagnostics, expression, eof);
}

int Parser::Facts::unary_operator_precedence(Kind kind)
{
    switch (kind) {
    case Kind::plus_token:
    case Kind::minus_token:
    case Kind::bang_token:
        return 6;
    default:
        return 0;
    }
}

int Parser::Facts::binary_operator_precedence(Kind kind)
{
    switch (kind) {
    case Kind::star_token:
    case Kind::forward_slash_token:
        return 5;
    case Kind::plus_token:
    case Kind::minus_token:
        return 4;
    case Kind::double_equals_token:
    case Kind::bang_equals_token:
        return 3;
    case Kind::double_ampersand_token:
        return 2;
    case Kind::double_pipe_token:
        return 1;
    default:
        return 0;
    }
}

Kind Parser::Facts::keyword_kind(std::string &text)
{
    if (text == "true")
        return Kind::true_keyword;
    if (text == "false")
        return Kind::false_keyword;
    return Kind::identifier_token;
}
