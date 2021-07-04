#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <variant>

#include "kind.h"

typedef long long unsigned int size;
using Value = std::variant<size, int, float, bool, const char *, std::string, BoundNodeKind>;

#endif // TYPEDEF_H