// The MIT License (MIT)

// Copyright (c) Immo Landwerth

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// https://github.com/terrajobst/minsk
// https://youtu.be/wgHIkdUQbp0

#include <iostream>
#include <variant>
#include <string>
#include <vector>
#include <memory>

typedef unsigned int size;

bool is_digit(char c);
size char_to_digit(char c);
bool string_to_size(const std::string &str, size *out);

template <typename... Args>
[[noreturn]] void runtime_error(const char* fmt, Args... args)
{
    printf(fmt, args...);
    exit(1);
}

// https://stackoverflow.com/a/26221725/13169325
template <typename... Args>
std::string format(const std::string &format, Args... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
    if (size_s <= 0)
        throw std::runtime_error("Error during formatting.");
    auto size = static_cast<size_t>(size_s);
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1);
}

enum Kind: int
{
    number,
    space,
    plus,
    minus,
    star,
    forward_slash,
    open_paren,
    close_parent,
    error,
    eof,
    number_expr,
    binary_expr,
    paren_expr
};

constexpr const char* kinds[] = {
    "number",
    "space",
    "plus",
    "minus",
    "star",
    "forward_slash",
    "open_paren",
    "close_parent" ,
    "error",
    "eof",
    "number_expr",
    "binary_expr",
    "paren_expr"
};

struct Token
{
    using Value = std::variant<size, int, float, bool, const char *, std::string>;

    Kind kind = Kind::error;
    size position = 0;
    std::string text;
    Value value;

    Token() {}
    Token(Kind kind, size position, std::string text, Value value) : kind(kind), position(position), text(text), value(value) {}
};

struct Lexer
{
    std::string text;
    size position = 0;
    std::vector<std::string> errors;

    Lexer(std::string text) : text(text), position(0) {}

    char current_char()
    {
        if (position >= text.length())
            return '\0';
        return text[position];
    }

    void next_char()
    {
        position++;
    }

    Token next_token()
    {
        if (position >= text.length())
            return Token(Kind::eof, position, "\0", nullptr);

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
                errors.push_back(format("[ERROR] the number '%s' isn't valid unsigned int", text.c_str()));
            return Token(Kind::number, start, text, value);
        }

        if (current == ' ')
        {
            size start = position;
            next_char();
            while (current_char() == ' ')
                next_char();
            size length = position - start;
            std::string text = this->text.substr(start, length);
            return Token(Kind::space, start, text, nullptr);
        }

        int temp = position;
        position++;

        if (current == '+') return Token(Kind::plus,          temp, "+", nullptr);
        if (current == '-') return Token(Kind::minus,         temp, "-", nullptr);
        if (current == '*') return Token(Kind::star,          temp, "*", nullptr);
        if (current == '/') return Token(Kind::forward_slash, temp, "/", nullptr);
        if (current == '(') return Token(Kind::open_paren,    temp, "(", nullptr);
        if (current == ')') return Token(Kind::close_parent,  temp, ")", nullptr);

        errors.push_back(format("[ERROR] bad input char: '%c'", current));
        return Token(Kind::error, temp, text.substr(temp, 1), nullptr);
    }
};

struct Node
{
    Kind kind = Kind::error;
};

struct Expression : Node
{
    virtual ~Expression() = default;
};

struct NumberExpr : Expression
{
    Kind kind;
    Token number;

    NumberExpr(Token number) : kind(Kind::number_expr), number(number) {}
};

struct BinaryExpr : Expression
{
    Expression* left;
    Token op;
    Expression* right;
    Kind kind;

    BinaryExpr(Expression* left, Token op, Expression* right) : left(left), op(op), right(right), kind(Kind::binary_expr) {}
};

struct ParenExpr : Expression
{
    Token open_paren;
    Expression* expr;
    Token close_paren;
    Kind kind;

    ParenExpr(Token open_paren, Expression* expr, Token close_paren)
        : open_paren(open_paren), expr(expr), close_paren(close_paren), kind(Kind::paren_expr) {}
};

struct Tree
{
    std::vector<std::string> errors;
    Expression* root;
    Token eof;

    Tree(const std::vector<std::string>& errors, Expression* root, Token eof) : errors(errors), root(root), eof(eof) {}

    Tree parse(std::string text);
};

struct Parser
{
    std::vector<Token> tokens;
    std::vector<std::string> errors;
    size position = 0;

    Parser(std::string text)
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

    Token peek(size offset)
    {
        size index = position + offset;
        if (index >= tokens.size())
            return tokens[tokens.size() - 1];
        return tokens[index];
    }

    Token current()
    {
        return peek(0);
    }

    Token next_token()
    {
        Token cur = current();
        position++;
        return cur;
    }

    Token match(Kind kind)
    {
        Token cur = current();
        if (cur.kind == kind)
        {
            return next_token();
        }
        errors.push_back(format("[ERROR] unexpected token: <%s>, expected <%s>", kinds[cur.kind], kinds[kind]));
        return Token(kind, cur.position, "\0", nullptr);
    }

    Expression* parse_expr()
    {
        return parse_term();
    }

    Expression* parse_primary()
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

    Tree parse()
    {
        Expression* expression = parse_term();
        Token eof = match(Kind::eof);
        return Tree(errors, expression, eof);
    }

    Expression* parse_term()
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

    Expression* parse_factor()
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
};

Tree Tree::parse(std::string text)
{
    Parser parser(text);
    return parser.parse();
}

struct Eval
{
    Expression* root;

    Eval(Expression* root) : root(root) {}

    size evaluate()
    {
        return evaluate_expr(root);
    }

    size evaluate_expr(Expression* expr)
    {
        if (NumberExpr* number_expr = dynamic_cast<NumberExpr*>(expr))
        {
            return std::get<size>(number_expr->number.value);
        }
        if (BinaryExpr* binary_expr = dynamic_cast<BinaryExpr*>(expr))
        {
            size left = evaluate_expr(binary_expr->left);
            size right = evaluate_expr(binary_expr->right);

            if (binary_expr->op.kind == Kind::plus)          return left + right;
            if (binary_expr->op.kind == Kind::minus)         return left - right;
            if (binary_expr->op.kind == Kind::star)          return left * right;
            if (binary_expr->op.kind == Kind::forward_slash) return left / right;

            runtime_error("unexpected binary operator: %s\n", kinds[binary_expr->kind]);
        }
        if (ParenExpr* paren_expr = dynamic_cast<ParenExpr*>(expr))
        {
            return evaluate_expr(paren_expr->expr);
        }
        runtime_error("unexpected expr: %s\n", kinds[expr->kind]);
    }
};

bool is_digit(char c)
{
    return 48 <= c && c <= 57;
}

size char_to_digit(char c)
{
    return c - 48;
}

bool string_to_size(const std::string &str, size *out)
{
    size position = 0;
    while (position < str.length())
    {
        size digit = char_to_digit(str[position]);
        if ((*out * 10 + digit) < *out)
        {
            return false;
        }
        *out = *out * 10 + digit;
        position++;
    }
    return true;
}

int main()
{
    while (std::cin)
    {
        printf("> ");
        std::string input;
        std::getline(std::cin, input);
        if (input.empty())
        {
            continue;
        }
        Tree tree = tree.parse(input);
        if (!tree.errors.empty())
        {
            for (auto &err : tree.errors)
            {
                printf("%s\n", err.c_str());
            }
        }
        else
        {
            Eval eval(tree.root);
            size result = eval.evaluate();
            printf("%d\n", result);
        }
    }
    return 0;
}
