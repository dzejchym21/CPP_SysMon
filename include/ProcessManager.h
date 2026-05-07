#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H
#include "Process.h"
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
public:
    void refresh();
    std::map<int, Process> getProcesses() const;
    std::vector<const Process*> getProcessesSnapshot(SortCategory sortCat, bool ascending);
};



#endif
