#pragma once

#include <cstdint>
#include <iostream>
#include <iomanip>

struct SysInfo_t
{
    uint64_t cpuTotalTime = 0;
    uint64_t cpuIdelTime = 0;
    uint64_t totalMemory = 0;
    uint64_t availableMem = 0;

    // std::ostream& operator<<(std::ostream& out){
    //     out << "CPU: %.2f %%" << static_cast<float>(cpuTotalTime-cpuIdelTime)/cpuTotalTime
    //         << "Mem: %.2f %%" << static_cast<float>(totalMemory-availableMem)/totalMemory;
    //     return out;
    // }
};

inline std::ostream& operator<<(std::ostream& out, const SysInfo_t& info) {
    double cpu_usage = 0.0;
    if (info.cpuTotalTime > 0) {
        cpu_usage = 100.0 * (static_cast<double>(info.cpuTotalTime - info.cpuIdelTime) 
                             / info.cpuTotalTime);
    }
    double mem_usage = 0.0;
    if (info.totalMemory > 0) {
        mem_usage = 100.0 * (static_cast<double>(info.totalMemory - info.availableMem) 
                             / info.totalMemory);
    }
    out << std::fixed << std::setprecision(2)
        << "CPU: " << cpu_usage << "% "
        << "Mem: " << mem_usage << "%";
    return out;
}

struct CpuSample_t
{
    uint64_t totalTime[2] = {0, 0};
    uint64_t idleTime[2] = {0, 0};
    bool p = 0;
};
