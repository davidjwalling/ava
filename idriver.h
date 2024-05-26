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
