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

