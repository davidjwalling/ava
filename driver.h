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
