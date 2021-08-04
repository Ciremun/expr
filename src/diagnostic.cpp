#include "diagnostic.h"
#include "typedef.h"
#include "util.h"

Diagnostic::Diagnostic(TextSpan *span, std::string message)
    : span(span), message(message) {}

void DiagnosticBag::report(TextSpan *span, std::string &message)
{
    Diagnostic *diagnostic = new Diagnostic(span, message);
    content.push_back(diagnostic);
}

void DiagnosticBag::report_invalid_number(TextSpan *span, std::string text, size_t type)
{
    std::string message = format("the number '%s' isn't valid %s", text.c_str(), variant_types[type]);
    report(span, message);
}

void DiagnosticBag::report_bad_char(size_t position, char character)
{
    TextSpan *span = new TextSpan(position, 1);
    std::string message = format("bad input char: '%c'", character);
    report(span, message);
}

void DiagnosticBag::report_unexpected_token(TextSpan *span, Kind error_kind, Kind expected_kind)
{
    std::string message = format("unexpected token: <%s>, expected <%s>", kinds[error_kind], kinds[expected_kind]);
    report(span, message);
}

void DiagnosticBag::report_undefined_unary_operator(TextSpan *span, std::string text, size_t type)
{
    std::string message = format("Unary operator <%s> is not defined for type <%s>", text.c_str(), variant_types[type]);
    report(span, message);
}

void DiagnosticBag::report_undefined_binary_operator(TextSpan *span, std::string text, size_t left_type, size_t right_type)
{
    std::string message = format("Binary operator <%s> is not defined for types <%s> and <%s>", text.c_str(), variant_types[left_type], variant_types[right_type]);
    report(span, message);
}

void DiagnosticBag::report_undefined_name(TextSpan *span, std::string name)
{
    std::string message = format("Variable <%s> doesn't exist", name.c_str());
    report(span, message);
}
