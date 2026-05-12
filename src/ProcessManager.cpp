#include "../include/ProcessManager.h"

#include <algorithm>
#include <unordered_set>
#include <vector>
#include <ranges>
#include "ProcParser.h"

void ProcessManager::refresh() {
    std::vector<int> currentPids = ProcParser::getAllPids();

    std::unordered_set<int> alivePids(currentPids.begin(), currentPids.end());
    for (int pid: currentPids) {
        if (processes.contains(pid)) {
            ProcessData dynamicData;
            ProcParser::fillDynamicData(pid, dynamicData);
            processes.at(pid).update(dynamicData);
        } else {
            processes.emplace(pid, ProcParser::buildProcess(pid));
        }
    }

    for (auto it = processes.begin(); it != processes.end(); ) {
        if (alivePids.find(it->first) == alivePids.end()) {
            it = processes.erase(it);
        } else {
            ++it;
        }
    }
}

std::map<int, Process> ProcessManager::getProcesses() const {
    return processes;
}

std::vector<const Process*> ProcessManager::getProcessesSnapshot(SortCategory sortCat, bool ascending) {
    std::vector<const Process*> vec;
    vec.reserve(processes.size());

    for (auto const& [pid, proc] : processes) {
        vec.push_back(&proc);
    }

    auto compare = [ascending](auto a, auto b) {
        return ascending ? (a < b) : (a > b);
    };

    switch (sortCat) {
        case SortCategory::CPU:
            std::ranges::sort(vec, compare, [](const Process* p) {return p->getCpuUsage();});
            break;
        case SortCategory::MEM:
            std::ranges::sort(vec, compare, [](const Process* p) {return p->getMemoryUsage();});
            break;
        case SortCategory::PID:
            std::ranges::sort(vec, compare, [](const Process* p) {return p->getPid();});
            break;
        case SortCategory::STATE:
            std::ranges::sort(vec, compare, [](const Process* p) {return p->getState();});
            break;
        case SortCategory::THREADS:
            std::ranges::sort(vec, compare, [](const Process* p) {return p->getThreads();});
            break;
        case SortCategory::TIME:
            std::ranges::sort(vec, compare, [](const Process* p) {return p->getTotalSeconds();});
            break;
        case SortCategory::NAME:
            std::ranges::sort(vec, [ascending](const Process* a, const Process* b) {
                std::string_view s1 = a->getName();
                std::string_view s2 = b->getName();

                auto cmp = [](unsigned char c1, unsigned char c2) {
                    return std::tolower(c1) < std::tolower(c2);
                };

                if (ascending) {
                    return std::ranges::lexicographical_compare(s1, s2, cmp);
                } else {
                    return std::ranges::lexicographical_compare(s2, s1, cmp);
                }
            });
            break;
        case SortCategory::USER:
            std::ranges::sort(vec, [ascending](const Process* a, const Process* b) {
                std::string_view s1 = a->getUser();
                std::string_view s2 = b->getUser();

                auto cmp = [](unsigned char c1, unsigned char c2) {
                    return std::tolower(c1) < std::tolower(c2);
                };

                if (ascending) {
                    return std::ranges::lexicographical_compare(s1, s2, cmp);
                } else {
                    return std::ranges::lexicographical_compare(s2, s1, cmp);
                }
            });
            break;
        default:
            break;
    }

    return vec;
}

void ProcessManager::updateCpuStats() {
    std::vector<CpuData> currentCpuStats = ProcParser::getAllCpus();

    if (lastCpuStats.empty()) {
        lastCpuStats = currentCpuStats;
        cpuUsages.resize(currentCpuStats.size(), 0.0);
        return;
    }

    for (int i = 0; i < currentCpuStats.size(); i++) {
        long deltaTotal = currentCpuStats[i].getTotalTime() - lastCpuStats[i].getTotalTime();
        long deltaIdle = currentCpuStats[i].getIdleTime() - lastCpuStats[i].getIdleTime();

        if (deltaTotal > 0) {
            cpuUsages[i] = (1.0 - static_cast<double>(deltaIdle) / deltaTotal) * 100.0;
        }

        lastCpuStats = currentCpuStats;
    }
}