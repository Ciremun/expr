#include "text_span.h"

TextSpan::TextSpan(size_t start, size_t length)
    : start(start), length(length), end(start + length) {}
