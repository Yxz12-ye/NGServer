#include "l_GetSysInfo.hpp"

#include "iostream"

void l_GetSysInfo::readCpuStat() {
    std::ifstream state("/proc/stat");
    if (!state.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return;
    }
    std::string cpu_lable;
    CpuState_t cpuState;
    state >> cpu_lable;
    state >> cpuState;
    m_cpuSample.idleTime[static_cast<int>(m_cpuSample.p)]=cpuState.idle;
    m_cpuSample.totalTime[static_cast<int>(m_cpuSample.p)]=cpuState.calcTotalTime();
    m_cpuSample.p = !m_cpuSample.p;

    state.close();
}

l_GetSysInfo::l_GetSysInfo() {
    readCpuStat();
}

SysInfo_t l_GetSysInfo::getSysInfo() {
    return SysInfo_t();
}

std::istream& operator>>(std::istream& input, CpuState_t& cpu) {
    input 
    >> cpu.user 
    >> cpu.nice 
    >> cpu.system 
    >> cpu.idle 
    >> cpu.iowait 
    >> cpu.irq 
    >> cpu.softirq 
    >> cpu.steal;
    return input;
}
