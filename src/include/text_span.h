#ifndef TEXT_SPAN_H
#define TEXT_SPAN_H

#include <cstddef>

struct TextSpan {
    size_t start;
    size_t length;
    size_t end;

    TextSpan(size_t start, size_t length);
};

#endif // TEXT_SPAN_H
