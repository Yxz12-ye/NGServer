#ifdef __linux__

#pragma once

#include "../BaseUtils.hpp"
#include "fstream"

struct MemInfo_t
{
    uint64_t totalMem;
    uint64_t availableMem;
};

class l_GetSysInfo : public BaseUtils {
private:
    CpuSample_t m_cpuSample;
    MemInfo_t m_memInfo;
    void readCpuStatus();
    void readMemInfo();

public:
    l_GetSysInfo(/* args */);
    ~l_GetSysInfo() = default;

    SysInfo_t getSysInfo() override;
};

struct CpuState_t {
    uint64_t user;     // 用户态nice<=0消耗CPU时间(进程nice默认为0)
    uint64_t nice;     // 用户态nice>0低优先进程消耗CPU时间
    uint64_t system;   // 内核态消耗的CPU时间
    uint64_t idle;     // 空闲时间
    uint64_t iowait;   // 等待IO完成的时间(core 2.6+)
    uint64_t irq;      // 硬件中断处理事件
    uint64_t softirq;  // 软件中断处理时间
    uint64_t steal;    // 虚拟化环境时间

    uint64_t calcTotalTime() { return user + nice + system + idle + irq + softirq + steal; }
};

std::istream& operator>>(std::istream&, CpuState_t&);

#endif