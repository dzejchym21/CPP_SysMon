#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H
#include "Process.h"
#include <unordered_map>
#include <vector>

enum class SortCategory {
    CPU,
    MEM,
    PID,
    PPID,
    STATE,
    NICE,
    NAME,
    USER,
    OTHER
};


class ProcessManager {
private:
    std::unordered_map<int, Process> processes;
public:
    void refresh();
    std::unordered_map<int, Process> getProcesses() const;
    std::vector<const Process*> getProcessesSnapshot(SortCategory sortCat, bool ascending);
};



#endif
