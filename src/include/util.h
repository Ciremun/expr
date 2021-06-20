#ifndef UTIL_H
#define UTIL_H

#include <memory>
#include <string>
#include <cstdio>
#include <stdexcept>

bool is_digit(char c);
size_t char_to_digit(char c);
bool string_to_size(const std::string &str, size_t *out);

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
    auto size = static_cast<size_t>(size_s);
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1);
}
#endif // UTIL_H