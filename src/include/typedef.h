#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <variant>

#include "kind.h"

typedef long long int size;
typedef unsigned long long int usize;
using Value = std::variant<size, usize, float, bool, const char *, std::string, BoundNodeKind>;

enum variant_type: int
{
    size_vt,
    usize_vt,
    float_vt,
    bool_vt,
    const_char_ptr_vt,
    std_string_vt,
    BoundNodeKind_vt
};

constexpr const char* variant_types[] = {
    "size",
    "usize",
    "float",
    "bool",
    "const char *",
    "std::string",
    "BoundNodeKind"
};

#endif // TYPEDEF_H