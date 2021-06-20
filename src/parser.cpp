#include "parser.h"

Parser::Parser(std::string text)
{
    std::vector<Token> tokens;
    Lexer lexer(text);
    Token token;
    do
    {
        token = lexer.next_token();
        if (token.kind != Kind::space && token.kind != Kind::error)
        {
            tokens.push_back(token);
        }
    } while (token.kind != Kind::eof);
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

Token Parser::match(Kind kind)
{
    Token cur = current();
    if (cur.kind == kind)
    {
        return next_token();
    }
    errors.push_back(format("[ERROR] unexpected token: <%s>, expected <%s>", kinds[cur.kind], kinds[kind]));
    return Token(kind, cur.position, "\0", nullptr);
}

Expression* Parser::parse_expr()
{
    return parse_term();
}

Expression* Parser::parse_primary()
{
    if (current().kind == Kind::open_paren)
    {
        Token left = next_token();
        Expression* expr = parse_expr();
        Token right = match(Kind::close_parent);
        return new ParenExpr(left, expr, right);
    }

    Token number = match(Kind::number);
    return new NumberExpr(number);
}

Tree Parser::parse()
{
    Expression* expression = parse_term();
    Token eof = match(Kind::eof);
    return Tree(errors, expression, eof);
}

Expression* Parser::parse_term()
{
    Expression* left = parse_factor();
    Token cur = current();

    while (cur.kind == Kind::plus || cur.kind == Kind::minus)
    {
        Token op = next_token();
        Expression* right = parse_factor();
        left = new BinaryExpr(left, op, right);
        cur = current();
    }

    return left;
}

Expression* Parser::parse_factor()
{
    Expression* left = parse_primary();
    Token cur = current();

    while (cur.kind == Kind::star || cur.kind == Kind::forward_slash)
    {
        Token op = next_token();
        Expression* right = parse_primary();
        left = new BinaryExpr(left, op, right);
        cur = current();
    }

    return left;
}