#define NOBUILD_IMPLEMENTATION
#include "nobuild.h"

#define CFLAGS "-Wall", "-Wextra", "-pedantic", "-std=c++17", "-I./src/include/", "-oexpr"
#define MSVC_FLAGS "/std:c++17", "/Feexpr.exe", "/Isrc/include/"


void format()
{
    CMD("astyle", "src/*.cpp", "src/*.h", "-n", "-r", "--style=kr");
}

void build()
{
    char *cxx = getenv("CXX");
#ifdef _WIN32
    if (cxx == NULL || strcmp(cxx, "cl") == 0 || strcmp(cxx, "cl.exe") == 0)
        CMD("cl.exe", "src/main.cpp", "src/expr.cpp", MSVC_FLAGS);
    else
        CMD(cxx, CFLAGS, "src/main.cpp", "src/expr.cpp");
#else
    if (cxx == NULL)
        CMD("g++", CFLAGS, "src/main.cpp", "src/expr.cpp");
    else
        CMD(cxx, CFLAGS, "src/main.cpp", "src/expr.cpp");
#endif
}

void run()
{
#ifdef _WIN32
    CMD(".\\expr.exe");
#else
    CMD("./expr");
#endif
}

void process_args(char **argv)
{
    if (strcmp(argv[1], "run") == 0) {
        build();
        run();
    } else if (strcmp(argv[1], "format") == 0) {
        format();
    }
    exit(0);
}

int main(int argc, char **argv)
{
    if (argc > 1)
        process_args(argv);
    else
        build();
}