#ifndef UTIL_H
#define UTIL_H

#include <memory>
#include <string>
#include <cstdio>
#include <stdexcept>
#include <type_traits>

#include "typedef.h"

bool is_letter(char c);
bool is_digit(char c);
size char_to_digit(char c);
bool string_to_size(const std::string &str, usize *out);

template <typename... Args>
[[noreturn]] void runtime_error(const char *fmt, Args... args)
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
    auto sz = static_cast<size>(size_s);
    auto buf = std::make_unique<char[]>(sz);
    std::snprintf(buf.get(), sz, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + sz - 1);
}

template <typename T>
using base_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

#endif // UTIL_H