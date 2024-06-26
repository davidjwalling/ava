# Ava

A multi-protocol server for various operation systems.

## Download, Build and Debug

### Linux or macOS
```
$ cd ~
$ mkdir repos && cd repos
$ git clone https://github.com/davidjwalling/ava.git
$ cd ava
```

### Build Using CMake

```
$ cd ~/repos/ava
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ sudo make install
$ cd ..
```

### Debugging Using Visual Studio Code

```
$ code .
```

On Linux, choose "(gdb) Ava" or "(gdb) Testava".
On macOS, choose "(lldb) Ava" or "(lldb) Testava".

![macos-01.png](images/macos-01.png)

## Code Elements

### The General Interface API (iapi.h)

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

### The IDriver Public Interface (idriver.h)

The IDriver interface declares public methods into the Driver implementation.

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

### The Private Driver Declaration (driver.h)

The Driver class implementation defines public and private methods.
Private Init and Reset methods allocate and initialize or release and reinitialize, respectively.
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

### The Private Driver Implmentation (driver.cpp)

The single instance of the Driver class is instantiated in driver.cpp as theDriver.
A reference to this is returned by TheDriver.
The address of the instance is returned by TheDriverPtr.
Other methods are merely placeholders for now.

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
```
```
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
#include "iapi.h"               //  __cdecl
#include "idriver.h"            //  IDriver

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

### The Test Program (testava.cpp)

The test program will test all algorithms implemented in the library.

```
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
```

### Ignoring Build Output When Updating The Repository (.gitignore)

```
build/
```

### Visual Studio Code Debug Configurations (.vscode/launch.json)

```
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Ava",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceRoot}/build/ava",
            "args": [],
            "stopAtEntry": true,
            "cwd": "${workspaceRoot}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb"
        },
        {
            "name": "(gdb) Testava",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceRoot}/build/testava",
            "args": [],
            "stopAtEntry": true,
            "cwd": "${workspaceRoot}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb"
        },
        {
            "name": "(lldb) Ava",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceRoot}/build/ava",
            "args": [],
            "stopAtEntry": true,
            "cwd": "${workspaceRoot}",
            "environment": [],
            "externalConsole": true,
            "MIMode": "lldb"
        },
        {
            "name": "(lldb) Testava",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceRoot}/build/testava",
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
