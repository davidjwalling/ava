#include "idriver.h"

#include <iostream>             //  getline, cin
#include <string>               //  string

using namespace std;

namespace ava {
    namespace command {
        constexpr const char* exit = "exit";
    }
}

int __cdecl main(int argc, char* argv[])
{
    IDriver& driver = TheDriver();

    if (driver.Start(argc, argv)) {
        string line;
        while (getline(cin, line) && line != ava::command::exit);
        driver.Stop();
    }

    return driver.Result();
}
