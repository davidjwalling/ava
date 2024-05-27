#include "iapi.h"

#include <iostream>

using namespace std;

bool _dump = false;
bool _all = false;

void runTests()
{
    cout << endl;
    cout << "Tests Completed" << endl;
}

int usage()
{
    cout << "Usage: testava [all] [dump]" << endl;
    return 0;
}

int __cdecl main(int argc, char* argv[])
{
    cout << "Ava Test Program [0.X]" << endl;
    cout << "Copyright 2010 David J. Walling. MIT License." << endl;

    if (argc > 3) {
        return usage();
    }
    if (argc > 2) {
        if ((string(argv[1]) == "all" && string(argv[2]) == "dump") || (string(argv[1]) == "dump" && string(argv[2]) == "all")) {
            _all = true;
            _dump = true;
        } else {
            return usage();
        }
    } else if (argc > 1) {
        if (string(argv[1]) == "all") {
            _all = true;
        } else if (string(argv[1]) == "dump") {
            _dump = true;
        } else {
            return usage();
        }
    }
    runTests();
    return 0;
}
