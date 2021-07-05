#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <variant>

#include "kind.h"

typedef long long int size;
typedef unsigned long long int usize;
using Value = std::variant<size, usize, float, bool, const char *, std::string, BoundNodeKind>;

#endif // TYPEDEF_H