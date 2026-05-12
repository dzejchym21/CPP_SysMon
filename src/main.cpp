#include <iostream>
#include <clocale>
#include <curses.h>
#include <chrono>
#include <thread>
#include "DisplayManager.h"

int main() {
    setlocale(LC_ALL, "");
    DisplayManager dm;
    ProcessManager pm;

    bool running = true;
    bool needsRedraw = true;
    auto lastRefreshTime = std::chrono::steady_clock::now();
    const auto refreshInterval = std::chrono::seconds(1);
    SortCategory currentSort = SortCategory::CPU;
    bool ascending = false;

    auto updateSort = [&](SortCategory sortCat) {
        if (currentSort == sortCat) {
            ascending = !ascending;
        } else {
            currentSort = sortCat;
            ascending = false;
        }
        needsRedraw = true;
    };

    pm.updateCpuStats();
    pm.refresh();

    while (running) {
        auto currentTime = std::chrono::steady_clock::now();

        if (currentTime - lastRefreshTime >= refreshInterval) {
            pm.updateCpuStats();
            pm.refresh();
            lastRefreshTime = currentTime;
            needsRedraw = true;
        }

        int ch = getch();
        switch (ch) {
            case 'q':
                running = false;
                break;
            case 'm':
                updateSort(SortCategory::MEM);
                break;
            case 'c':
                updateSort(SortCategory::CPU);
                break;
            case 'n':
                updateSort(SortCategory::NAME);
                break;
            case 'p':
                updateSort(SortCategory::PID);
                break;
            case 's':
                updateSort(SortCategory::STATE);
                break;
            case 'u':
                updateSort(SortCategory::USER);
                break;
            case 't':
                updateSort(SortCategory::TIME);
                break;
        }

        if (needsRedraw && running) {
            erase();
            dm.drawCpuDashboard(pm.getAllCpuStats());
            dm.render(pm.getProcessesSnapshot(currentSort, ascending), currentSort, ascending);
            needsRedraw = false;
        }
    }
}