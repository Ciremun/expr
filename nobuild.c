#define NOBUILD_IMPLEMENTATION
#include "nobuild.h"

#define CFLAGS "-Wall", "-Wextra", "-pedantic", "-std=c++17", "-I./src/include/"
#define MSVC_FLAGS "/std:c++17", "/Isrc/include/"

#define PROCESSES_CAPACITY 256

#define ASYNC_BUILD(msvc, object_file_extenstion, ...)                              \
    do                                                                              \
    {                                                                               \
        async_obj_foreach_file_in_dirs(msvc,                                        \
            "src/", "src/syntax/", "src/binding/", NULL);                           \
        Cstr_Array line = cstr_array_make(__VA_ARGS__, NULL);                       \
        FOREACH_FILE_IN_DIR(file, ".",                                              \
        {                                                                           \
            if (ENDS_WITH(file, object_file_extenstion))                            \
                line = cstr_array_append(line, strdup(file));                       \
        });                                                                         \
        Cmd cmd = {.line = line};                                                   \
        INFO("CMD: %s", cmd_show(cmd));                                             \
        cmd_run_sync(cmd);                                                          \
    } while (0)

#define ASYNC_OBJ_FOREACH_FILE_IN_DIR(directory,                                    \
                                      source_extension,                             \
                                      msvc)                                         \
    do                                                                              \
    {                                                                               \
        FOREACH_FILE_IN_DIR(file, directory,                                        \
            {                                                                       \
                if (ENDS_WITH(file, source_extension))                              \
                {                                                                   \
                    Cstr src = CONCAT(directory, strdup(file));                     \
                    Cstr obj = msvc ?                                               \
                        CONCAT(NOEXT(file), ".obj") :                               \
                        CONCAT(NOEXT(file), ".o");                                  \
                    if (!PATH_EXISTS(obj)                                           \
                        ||                                                          \
                        (force_rebuild || is_path1_modified_after_path2(src, obj))) \
                    {                                                               \
                        Cstr_Array line = msvc ?                                    \
                            cstr_array_make(cxx, MSVC_FLAGS, src, "/c", NULL) :     \
                            cstr_array_make(cxx, CFLAGS, src, "-c", NULL);          \
                        Cmd cmd = {                                                 \
                            .line = line};                                          \
                        INFO("CMD: %s", cmd_show(cmd));                             \
                        proc[proc_count] = cmd_run_async(cmd, NULL, NULL);          \
                        proc_count++;                                               \
                    }                                                               \
                }                                                                   \
            });                                                                     \
    } while (0)

int force_rebuild = 0;
char *cxx;

void async_obj_foreach_file_in_dirs(int msvc, Cstr first, ...)
{
    Pid proc[PROCESSES_CAPACITY];
    size_t proc_count = 0;
    ASYNC_OBJ_FOREACH_FILE_IN_DIR(first, ".cpp", msvc);
    va_list args;
    va_start(args, first);
    for (Cstr directory = va_arg(args, Cstr);
         directory != NULL;
         directory = va_arg(args, Cstr))
    {
        ASYNC_OBJ_FOREACH_FILE_IN_DIR(directory, ".cpp", msvc);
    }
    va_end(args);
    for (size_t i = 0; i < proc_count; ++i)
        pid_wait(proc[i]);
}

void format()
{
    CMD("astyle", "src/*.cpp", "src/*.h", "-n", "-r", "--style=kr");
}

void build()
{
    cxx = getenv("CXX");
#ifdef _WIN32
    if (cxx == NULL || strcmp(cxx, "cl") == 0 || strcmp(cxx, "cl.exe") == 0)
    {
        cxx = "cl";
        ASYNC_BUILD(1, ".obj", "LINK", "/OUT:expr.exe");
    }
    else
    {
        ASYNC_BUILD(0, ".o", cxx, CFLAGS, "-oexpr");
    }
#else
    if (cxx == NULL)
        cxx = "g++";
    ASYNC_BUILD(0, ".o", cxx, CFLAGS, "-oexpr");
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

void process_args(int argc, char **argv)
{
    int rebuild_flag = 0;
    int run_flag = 0;
    int fmt_flag = 0;
    for (int i = 0; i < argc; ++i)
    {
        if (!rebuild_flag && strcmp(argv[i], "rebuild") == 0)
        {
            rebuild_flag = 1;
            force_rebuild = 1;
            build();
        }
        if (!run_flag && strcmp(argv[i], "run") == 0)
        {
            run_flag = 1;
            if (!rebuild_flag)
                build();
            run();
        }
        if (!fmt_flag && (strcmp(argv[i], "fmt") == 0 || strcmp(argv[i], "format") == 0))
        {
            fmt_flag = 1;
            format();
        }
    }
}

int main(int argc, char **argv)
{
    GO_REBUILD_URSELF(argc, argv);

    if (argc > 1)
        process_args(argc, argv);
    else
        build();
    return 0;
}