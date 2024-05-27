#include "iapi.h"

#include <iostream>

bool _dump = false;
bool _all = false;

void runTests()
{
    std::cout << std::endl;
    std::cout << "Tests Completed" << std::endl;
}

int usage()
{
    std::cout << "Usage: testava [all] [dump]" << std::endl;
    return 0;
}

int __cdecl main(int argc, char* argv[])
{
    std::cout << "Ava Test Program [0.X]" << std::endl;
    std::cout << "Copyright 2010 David J. Walling. MIT License." << std::endl;

    if (argc > 3) {
        return usage();
    }
    if (argc > 2) {
        if ((std::string(argv[1]) == "all" && std::string(argv[2]) == "dump") || (std::string(argv[1]) == "dump" && std::string(argv[2]) == "all")) {
            _all = true;
            _dump = true;
        } else {
            return usage();
        }
    } else if (argc > 1) {
        if (std::string(argv[1]) == "all") {
            _all = true;
        } else if (std::string(argv[1]) == "dump") {
            _dump = true;
        } else {
            return usage();
        }
    }
    runTests();
    return 0;
}
