#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include <vector>
#include <string>

#include "text_span.h"
#include "kind.h"

struct Diagnostic {
    TextSpan *span;
    std::string message;

    Diagnostic(TextSpan *span, std::string message);
};

struct DiagnosticBag {
    std::vector<Diagnostic*> content;

    void report(TextSpan *span, std::string &message);
    void report_invalid_number(TextSpan *span, std::string text, size_t type);
    void report_bad_char(size_t position, char character);
    void report_unexpected_token(TextSpan *span, Kind error_kind, Kind expected_kind);
    void report_undefined_unary_operator(TextSpan *span, std::string text, size_t type);
    void report_undefined_binary_operator(TextSpan *span, std::string text, size_t left_type, size_t right_type);
    void report_undefined_name(TextSpan *span, std::string name);
};

#endif // DIAGNOSTIC_H
