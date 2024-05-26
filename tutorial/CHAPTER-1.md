# CHAPTER-1.md

## Downloading Ava

```
user@linux: ~$ mkdir repos && cd repos
user@linux: ~/repos$ git clone https://github.com/davidjwalling/ava.git
user@linux: ~/repos$ cd ava
```

## Building Ava

```
user@linux: ~/repos/ava$ mkdir build && cd build
user@linux: ~/repos/ava/build$ cmake -DCMAKE_BUILD_TYPE=Debug ..
user@linux: ~/repos/ava/build$ sudo make install
user@linux: ~/repos/ava/build$ cd ..
```

## Debugging Ava Using Visual Studio Code

```
user@linux: ~/repos/ava$ code .
```
![macos-01.png](/images/macos-01.png)

## Sources for this Chapter

### The General Interface API Header (iapi.h)

On Windows, EXPORT is dllexport or dllimport as appropriate.
On Linux and macOS, EXPORT sets default symbol visibility.
On other platforms, EXPORT placeholds.
On non-Windows platforms, __cdecl placeholds.

```
#pragma once

#if defined(_WIN32)
#if defined(_WINDLL)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif
#else
#if defined(__linux__) || defined(__APPLE__)
#define EXPORT __attribute__((visibility("default")))
#else
#define EXPORT
#endif
#define __cdecl
#endif
```

### The Driver Interface Header (idriver.h)

The Driver class will implement top-level methods accessed by applications.
The IDriver interface declares only those methods required by applications.
All other Driver methods will be declared in the private implementation.

```
#pragma once

#include "iapi.h"

struct IDriver
{
    virtual bool Start(int argc, char* argv[]) = 0;
    virtual void Stop() = 0;
    virtual int Result() = 0;
};

EXPORT IDriver& TheDriver();
EXPORT IDriver* TheDriverPtr();
```

### The Private Driver Class Declaration (driver.h)

The Driver class implementation drives from the public IDriver interface.
Private Init and Reset methods allocate and initialize or release and reinitialize, repsectively.
Use the override keyword to validate inheritance at compilation time.

```
#pragma once

#include "idriver.h"

class Driver
    : public IDriver
{
    void Init();
    void Reset();
public:
    Driver();
    ~Driver();
    bool Start(int argc, char* argv[]) override;
    void Stop() override;
    int Result() override;
};
```

### The Private Driver Class Implmentation (driver.cpp)

The single instance of the Driver class is instantiated in driver.cpp as theDriver. A reference to this is returned by TheDriver. The address of the instance is returned by TheDriverPtr. Other methods are merely placeholders for now.

```
#include "driver.h"

Driver theDriver;

Driver::Driver()
{
    Init();
}

Driver::~Driver()
{
    Reset();
}

void Driver::Init()
{
}

void Driver::Reset()
{
    Init();
}

bool Driver::Start(int, char*[])
{
    return true;
}

void Driver::Stop()
{
}

int Driver::Result()
{
    return 0;
}

IDriver& TheDriver()
{
    return theDriver;
}

IDriver* TheDriverPtr()
{
    return &theDriver;
}
```

### The Application (ava.cpp)

At program entry, retrieve a reference to the Driver.
Start the Driver passing program arguments as parameters.
If initialization succeeds, accept user input on the mian thread until 'exit'.
Stop the Driver and return the Result.

```
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
```

### Helper Function Declarations (api.h)

The tokstrx returns the next token, removing leading characters.

```
#pragma once

#include "iapi.h"

EXPORT char* tokstrx(char* s, const char* d, const char* x, char** p);
```

### Helper Function Implementation (api.cpp)

```
#include "api.h"

char* tokstrx(char* s, const char* d, const char* x, char** p)
{
    char* t, * f;
    const char* e;
    if (p) {
        if (!s)
            s = *p;
        *p = nullptr;
    }
    if (!s || !(*s))
        return nullptr;
    t = s;
    if (d) {
        for (; *t; t++) {
            for (e = d; *e && *e != *t; e++);
            if (!(*e))
                break;
        }
        if (!(*t))
            return nullptr;
    }
    f = t;
    if (x) {
        for (; *t; t++) {
            for (e = x; *e && *t != *e; e++);
            if (*e) {
                *t = 0;
                if (p)
                    *p = t + 1;
                break;
            }
        }
    }
    return f;
}
```
### The Test Program (testava.cpp)

The test program will test all algorithms implemented in the library.

```
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
```

### Ignoring Build Output When Updating The Repository (.gitignore)

```
build/
```

### Debuging Ava Using Visual Studio Code (.vscode/launch.json)

```
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceRoot}/build/ava/ava",
            "args": [],
            "stopAtEntry": true,
            "cwd": "${workspaceRoot}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb"
        },
        {
            "name": "(lldb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceRoot}/build/ava/ava",
            "args": [],
            "stopAtEntry": true,
            "cwd": "${workspaceRoot}",
            "environment": [],
            "externalConsole": true,
            "MIMode": "lldb"
        }
    ]
}
```