#ifndef PROCESS_H
#define PROCESS_H
#include <optional>
#include <string>

struct ProcessData {
    int pid;
    int ppid;
    std::string name;
    std::string user;
    char state;
    int nice;
    int threads;
    long memoryUsage;
    long totalTime;
    double systemUpTime;
    double totalSeconds;
};


class Process {
private:
    int pid;
    int ppid;
    std::string name;
    std::string user;
    char state;
    long memoryUsage;
    int nice;
    int threads;
    double totalSeconds;

    // Fields to count the %CPU
    double cpuUsage;
    long lastTotalTime;
    double lastSystemUpTime;
public:
    explicit Process(const ProcessData& data);

    // Getters
    int getPid() const;
    int getPpid() const;
    int getNice() const;
    int getThreads() const;
    const std::string& getName() const;
    const std::string& getUser() const;
    char getState() const;
    double getCpuUsage() const;
    long getMemoryUsage() const;
    long getLastTotalTime() const;
    double getTotalSeconds() const;

    // Setters
    void setPpid(int id);
    void update(const ProcessData& data);
};



#endif
