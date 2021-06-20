#include "util.h"

bool is_digit(char c)
{
    return 48 <= c && c <= 57;
}

size char_to_digit(char c)
{
    return c - 48;
}

bool string_to_size(const std::string &str, size *out)
{
    size position = 0;
    while (position < str.length()) {
        size digit = char_to_digit(str[position]);
        if ((*out * 10 + digit) < *out) {
            return false;
        }
        *out = *out * 10 + digit;
        position++;
    }
    return true;
}