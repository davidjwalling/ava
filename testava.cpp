#include "api.h"

#include <cstdio>
#include <cstring>

namespace testava {
    const char* all = "all";
    const char* dump = "dump";
    const char* help = "help";
    const char* test = "test";
}

bool _dump = false;
bool _all = false;

void runTests()
{
    printf("\n");
    printf("Tests Completed\n");
}

int usage()
{
    printf("Usage: testava test [all] [dump]\n");
    return 0;
}

int __cdecl main(int argc, char* argv[])
{
    printf("Ava Test Program [0.X]\n");
    printf("Copyright 2010 David J. Walling. MIT License.\n");
    if (argc > 4 || (argc == 2 && !strcmp(argv[1], testava::help)))
        return usage();
    char* args[4] = { 0 };
    for (int n = 0; n < argc; n++)
        args[n] = argv[n];
    if (1 == argc) {
        char cmd[256] = { 0 };
        fflush(stdin);
        fgets(cmd, sizeof cmd - 1, stdin);
        char* nexttok = cmd;
        for (int n = 1; nexttok && n < 4; n++)
            args[n] = tokstrx(nexttok, " \t", " \t\n", &nexttok);
    }
    if (args[1] && !strcmp(args[1], testava::test)) {
        for (int n = 2; n < 4 && args[n]; n++) {
            if (!strcmp(args[n], testava::all))
                _all = true;
            else if (!strcmp(args[n], testava::dump))
                _dump = true;
        }
        runTests();
    } else
        return usage();
    return 0;
}
