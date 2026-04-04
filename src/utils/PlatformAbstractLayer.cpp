#include "PlatformAbstractLayer.hpp"

PlatformAbstractLayer::PlatformAbstractLayer() {
#ifdef _WIN32
#elif defined(__linux__)
    SysInfo = std::make_unique<l_GetSysInfo>();
#elif defined(__ALLPE__)
#endif
}

SysInfo_t PlatformAbstractLayer::GetSysInfo() {
    if (!SysInfo) return {};
    return SysInfo->getSysInfo();
}
