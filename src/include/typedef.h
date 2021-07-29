#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <variant>
#include <string>

#include "kind.h"

typedef long long int size;
typedef unsigned long long int usize;
using Value = std::variant<size, usize, float, bool, const char *, std::string, BoundNodeKind>;

const char *const variant_types[] = {
    "size",
    "usize",
    "float",
    "bool",
    "const char *",
    "std::string",
    "BoundNodeKind"
};

static_assert(sizeof(variant_types) / sizeof(variant_types[0]) == std::variant_size_v<Value>, "update variant_types[]");

#endif // TYPEDEF_H
