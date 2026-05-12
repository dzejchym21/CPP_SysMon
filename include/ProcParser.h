#ifndef PROCPARSER_H
#define PROCPARSER_H

#include <vector>
#include "Process.h"
#include "CpuData.h"

class ProcParser {
public:
    static std::vector<int> getAllPids();
    static std::vector<CpuData> getAllCpus();
    static void fillStaticData(int pid, ProcessData& pd);
    static void fillDynamicData(int pid, ProcessData& pd);
    static Process buildProcess(int pid);

};



#endif
