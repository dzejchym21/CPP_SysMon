#ifndef CPP_SYSMON_CPUDATA_H
#define CPP_SYSMON_CPUDATA_H

struct CpuData {
    long user, nice, system, idle, iowait, irq, softirq;

    long getTotalTime() const {
        return user + nice + system + idle + irq + softirq;
    }

    long getIdleTime() const {
        return idle + iowait;
    }
};

#endif
