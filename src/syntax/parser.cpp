#include "parser.h"

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
    errors = std::vector(lexer.errors.begin(), lexer.errors.end());
}

Token Parser::peek(size offset)
{
    size index = position + offset;
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
    errors.push_back(format("[ERROR] unexpected token: <%s>, expected <%s>", kinds[cur.kind], kinds[kind]));
    return Token(kind, cur.position, "\0", nullptr);
}

Expression* Parser::parse_expression(int parent_precedence)
{
    Expression* left;
    int unary_operator_precedence = Facts::unary_operator_precedence(current().kind);
    if (unary_operator_precedence != 0 && unary_operator_precedence >= parent_precedence) {
        Token op = next_token();
        Expression* operand = parse_expression(unary_operator_precedence);
        left = new UnaryExpr(op, operand);
    } else {
        left = parse_primary_expression();
    }

    while (1) {
        int precedence = Facts::binary_operator_precedence(current().kind);
        if (precedence == 0 || precedence <= parent_precedence)
            break;
        Token operator_token = next_token();
        Expression* right = parse_expression(precedence);
        left = new BinaryExpr(left, operator_token, right);
    }

    return left;
}

Expression* Parser::parse_primary_expression()
{
    if (current().kind == Kind::open_paren_token) {
        Token left = next_token();
        Expression* expr = parse_expression();
        Token right = match_token(Kind::close_paren_token);
        return new ParenExpr(left, expr, right);
    }

    Token number = match_token(Kind::number_token);
    return new LiteralExpr(number);
}

Tree Parser::parse()
{
    Expression* expression = parse_expression();
    Token eof = match_token(Kind::eof_token);
    return Tree(errors, expression, eof);
}

int Parser::Facts::binary_operator_precedence(Kind kind)
{
    switch (kind) {
    case Kind::star_token:
    case Kind::forward_slash_token:
        return 2;
    case Kind::plus_token:
    case Kind::minus_token:
        return 1;
    default:
        return 0;
    }
}

int Parser::Facts::unary_operator_precedence(Kind kind)
{
    switch (kind) {
    case Kind::plus_token:
    case Kind::minus_token:
        return 3;
    default:
        return 0;
    }
}
