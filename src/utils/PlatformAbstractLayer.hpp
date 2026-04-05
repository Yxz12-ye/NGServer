#pragma once

#include "./linux/l_GetSysInfo.hpp"
#include "memory"

class PlatformAbstractLayer
{
private:
    std::unique_ptr<BaseUtils> SysInfo;

public:
    PlatformAbstractLayer(/* args */);
    ~PlatformAbstractLayer() = default;

    SysInfo_t GetSysInfo();
};
