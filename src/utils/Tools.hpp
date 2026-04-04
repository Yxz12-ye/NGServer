#include <cstdint>

struct SysInfo_t
{
    uint64_t cpuTotalTime = 0;
    uint64_t cpuIdelTime = 0;
    uint64_t totalMemory = 0;
    uint64_t availableMem = 0;
};

struct CpuSample_t
{
    uint64_t totalTime[2] = {0, 0};
    uint64_t idleTime[2] = {0, 0};
    bool p = 0;
};

