#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <variant>
#include <string>
#include <unordered_map>

#include "kind.h"

typedef long long int size;
typedef unsigned long long int usize;
using Value = std::variant<std::nullptr_t, size, bool, std::string, BoundNodeKind>;
using Vars = std::unordered_map<std::string, Value>;

const char *const variant_types[] = {
    "std::nullptr_t",
    "size",
    "bool",
    "std::string",
    "BoundNodeKind",
};

static_assert(sizeof(variant_types) / sizeof(variant_types[0]) == std::variant_size_v<Value>, "update variant_types[]");

#endif // TYPEDEF_H
