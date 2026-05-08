#include "ProcParser.h"
#include "UserHelper.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <unistd.h>

using namespace std::filesystem;

namespace {
    constexpr std::string_view PROC_DIRECTORY = "/proc";
    constexpr std::string_view STATUS_FILE = "status";
    constexpr std::string_view STAT_FILE = "stat";
}

std::vector<int> ProcParser::getAllPids() {
    std::vector<int> pids;

    if (!exists(PROC_DIRECTORY) || !is_directory(PROC_DIRECTORY)) {
        return pids;
    }

    for (const auto& entry: directory_iterator(PROC_DIRECTORY)) {
        if (is_directory(entry)) {
            const std::string name = entry.path().filename().string();
            if (std::ranges::all_of(name, isdigit)) {
                try {
                    pids.emplace_back(std::stoi(name));
                } catch (...) {
                    continue;
                }
            }
        }
    }

    return pids;
}

Process ProcParser::buildProcess(int pid) {

    ProcessData pd;
    pd.pid = pid;
    pd.name = "Unknown";
    pd.state = 'U';
    pd.ppid = 0;
    pd.user = "Unknown";
    pd.memoryUsage = 0;

    fillStaticData(pid, pd);
    fillDynamicData(pid, pd);

    return Process(pd);
}

void ProcParser::fillStaticData(const int pid, ProcessData& pd) {
    std::ifstream file(path(PROC_DIRECTORY) / std::to_string(pid) / STATUS_FILE);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.compare(0, 5, "Name:") == 0) {
                pd.name = line.substr(6);
                pd.name.erase(0, pd.name.find_first_not_of(" \t"));
                pd.name.erase(pd.name.find_last_not_of(" \t\n\r") + 1);
            }
            else if (line.compare(0, 4, "Uid:") == 0) {
                std::stringstream ss(line.substr(5));
                int uid;
                ss >> uid;
                pd.user = UserHelper::getUserName(uid);
                break;
            }
        }
    }
}

void ProcParser::fillDynamicData(const int pid, ProcessData& pd) {
    static long pageSize = sysconf(_SC_PAGESIZE);

    std::ifstream file(path(PROC_DIRECTORY) / std::to_string(pid) / STAT_FILE);
    std::string line;
    if (getline(file, line)) {
        size_t lastBracket = line.find_last_of(')');
        if (lastBracket == std::string::npos) return;

        std::string dummy;
        std::stringstream ss(line.substr(lastBracket + 1));
        long utime, stime, rss_pages;
        ss >> pd.state;
        ss >> pd.ppid;

        for (size_t i = 0; i < 9; ++i) ss >> dummy;

        ss >> utime >> stime;
        pd.totalTime = utime + stime;

        for (size_t i = 0; i < 3; ++i) ss >> dummy;

        ss >> pd.nice >> pd.threads;

        for (size_t i = 0; i < 3; ++i) ss >> dummy;

        ss >> rss_pages;
        pd.memoryUsage = rss_pages * (pageSize / 1024);

        struct timespec ts;
        if (clock_gettime(CLOCK_BOOTTIME, &ts) == 0) {
            pd.systemUpTime = static_cast<double>(ts.tv_sec) + static_cast<double>(ts.tv_nsec) / 1e9;
        }
    }
}