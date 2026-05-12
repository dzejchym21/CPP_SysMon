#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H
#include "Process.h"
#include "CpuData.h"
#include <map>
#include <vector>

enum class SortCategory {
    CPU,
    MEM,
    PID,
    PPID,
    THREADS,
    TIME,
    STATE,
    NICE,
    NAME,
    USER,
    OTHER
};

class ProcessManager {
private:
    std::map<int, Process> processes;
    std::vector<double> cpuUsages;
    std::vector<CpuData> lastCpuStats;
public:
    void refresh();
    void updateCpuStats();
    std::map<int, Process> getProcesses() const;
    std::vector<const Process*> getProcessesSnapshot(SortCategory sortCat, bool ascending);
    std::vector<double> getAllCpuStats();
};



#endif
