#ifdef __linux__

#include "l_GetSysInfo.hpp"

#include "iostream"

void l_GetSysInfo::readCpuStatus() {
    std::ifstream state("/proc/stat");
    if (!state.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return;
    }
    std::string _cpu_lable;
    CpuState_t cpuState;
    state >> _cpu_lable;
    state >> cpuState;
    m_cpuSample.idleTime[static_cast<int>(m_cpuSample.p)]=cpuState.idle;
    m_cpuSample.totalTime[static_cast<int>(m_cpuSample.p)]=cpuState.calcTotalTime();
    m_cpuSample.p = !m_cpuSample.p;

    state.close();
}

void l_GetSysInfo::readMemInfo() {
    std::ifstream mem("/proc/meminfo");
    if (!mem.is_open()){
        std::cerr << "无法打开文件" << std::endl;
        return;
    }
    std::string _label[3]; std::string _unit; uint64_t _skip;
    mem >> _label[0] >> m_memInfo.totalMem >> _unit;
    mem >> _label[1] >> _skip >> _unit;
    mem >> _label[2] >> m_memInfo.availableMem >> _unit;

    mem.close();
}

l_GetSysInfo::l_GetSysInfo() {
    std::cerr << "l_GetSysInfo" << std::endl;
}

SysInfo_t l_GetSysInfo::getSysInfo() {
    readCpuStatus();
    readMemInfo();
    return {
        m_cpuSample.totalTime[!m_cpuSample.p]-m_cpuSample.totalTime[m_cpuSample.p],
        m_cpuSample.idleTime[!m_cpuSample.p]-m_cpuSample.idleTime[m_cpuSample.p],
        m_memInfo.totalMem,
        m_memInfo.availableMem
    };
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

#endif