#define NOBUILD_IMPLEMENTATION
#include "nobuild.h"

#define CFLAGS "-Wall", "-Wextra", "-std=c++17", "-pedantic"

int main(void)
{
    char* cc = getenv("cc");
#ifdef _WIN32
    if (cc == NULL || strcmp(cc, "cl") == 0 || strcmp(cc, "cl.exe") == 0)
        CMD("cl.exe", "expr.cpp", "/std:c++17", "/Feexpr.exe");
    else
        CMD(cc, CFLAGS, "expr.cpp", "-oexpr");
#else
    if (cc == NULL)
        CMD("g++", CFLAGS, "expr.cpp", "-oexpr");
    else
        CMD(cc, CFLAGS, "expr.cpp", "-oexpr");
#endif
}