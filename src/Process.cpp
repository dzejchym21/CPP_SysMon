#include "Process.h"
#include <iostream>
#include <unistd.h>

Process::Process(const ProcessData& data): pid(data.pid), ppid(data.ppid), name(data.name), user(data.user), state(data.state), memoryUsage(data.memoryUsage), nice(data.nice), threads(data.threads), cpuUsage(0.0f), totalSeconds(0.0), lastTotalTime(data.totalTime), lastSystemUpTime(data.systemUpTime) {}

void Process::update(const ProcessData& data) {
    static long hertz = sysconf(_SC_CLK_TCK);
    long deltaProcessTime = data.totalTime - lastTotalTime;
    double deltaTime = data.systemUpTime - lastSystemUpTime;

    this->totalSeconds = static_cast<double>(data.totalTime) / hertz;

    if (deltaTime > 0 && lastTotalTime > 0) {
        double processSeconds = static_cast<double>(deltaProcessTime) / hertz;
        cpuUsage = (processSeconds / deltaTime) * 100.0;
    }

    lastTotalTime = data.totalTime;
    lastSystemUpTime = data.systemUpTime;

    this->memoryUsage = data.memoryUsage;
    this->state = data.state;
    this->ppid = data.ppid;
    this->nice = data.nice;
}

// Getters
int Process::getPid() const {
    return pid;
}

int Process::getPpid() const {
    return ppid;
}

int Process::getNice() const {
    return nice;
}

int Process::getThreads() const {
    return threads;
}

const std::string& Process::getName() const {
    return name;
}

const std::string& Process::getUser() const {
    return user;
}

char Process::getState() const {
    return state;
}

double Process::getTotalSeconds() const {
    return totalSeconds;
}

double Process::getCpuUsage() const {
    return cpuUsage;
}

long Process::getMemoryUsage() const {
    return memoryUsage;
}

long Process::getLastTotalTime() const {
    return lastTotalTime;
}

// Setters
void Process::setPpid(int id) {
    ppid = id;
}