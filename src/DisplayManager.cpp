#include "DisplayManager.h"
#include <cmath>
#include <cstring>
#include <curses.h>
#include <vector>


DisplayManager::DisplayManager() {
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    keypad(stdscr, true);
    noecho();
    curs_set(0);
    cbreak();
    timeout(100);
}

DisplayManager::~DisplayManager() {
    endwin();
}

void DisplayManager::drawHeader(SortCategory currentSort, bool ascending) {
    attron(COLOR_PAIR(1) | A_BOLD);
    int currentX = 0;
    int row = 4;

    auto drawCol = [&](const char* label, int width, SortCategory colCat, bool leftAlign) {
        int pair = (currentSort == colCat && colCat != SortCategory::OTHER) ? 2 : 1;

        attron(COLOR_PAIR(pair) | A_BOLD);

        for (int i = 0; i < width; i++) {
            mvaddch(row, currentX + i, ' ');
        }

        const char* end = " ";
        if (pair == 2) {
            end = ascending ? "\u25B2" : "\u25BC";
        }

        if (leftAlign) {
            mvprintw(row, currentX , "%.*s", width, label);
        } else {
            int len = strlen(label);
            int startPos = (len > width) ? currentX : currentX + (width - len);
            mvprintw(row, startPos, "%.*s", width, label);
        }

        currentX += width;
        mvaddstr(row, currentX, end);
        currentX += 1;
        attroff(COLOR_PAIR(pair) | A_BOLD);
    };

    drawCol("PID", 8, SortCategory::PID, false);
    drawCol("PPID", 7, SortCategory::OTHER, false);
    drawCol("USER", 13, SortCategory::USER, true);
    drawCol("NICE", 8, SortCategory::OTHER, false);
    drawCol("ST", 2, SortCategory::STATE, false);
    drawCol("TH", 2, SortCategory::THREADS, false);
    drawCol("RES (kB)", 9, SortCategory::MEM, false);
    drawCol("%CPU", 5, SortCategory::CPU, false);
    drawCol("TIME+", 10, SortCategory::TIME, false);
    drawCol("COMMAND", 20, SortCategory::NAME, true);
    //mvprintw(0, 0, "%8s %8s %-15s %-10s %8s %-12s %-5s",
    //        "PID", "PPID", "COMMAND", "USER", "NICE", "RES (kB)", "%CPU");

    attroff(COLOR_PAIR(1) | A_BOLD);
}


void DisplayManager::render(const std::vector<const Process*>& processes, SortCategory currentSort, bool ascending) {
    //erase();
    //clear();
    drawHeader(currentSort, ascending);

    int row = 5;
    int maxRows = LINES - 1;
    for (const auto* proc : processes) {
        if (row > maxRows) break;

        double ts = proc->getTotalSeconds();
        int mins = static_cast<int>(ts) / 60;
        double secs = ts - (mins * 60);

        mvprintw(row, 0, "%8d %8d %-12.12s %8d %2c %2d %9ld %6.2f %3d:%05.2f %-15.15s",
            proc->getPid(),
            proc->getPpid(),
            proc->getUser().c_str(),
            proc->getNice(),
            proc->getState(),
            proc->getThreads(),
            proc->getMemoryUsage(),
            proc->getCpuUsage(),
            mins, secs,
            proc->getName().c_str());

        row++;
    }
    refresh();
}

void DisplayManager::drawCpuDashboard(const std::vector<double>& usages) {
    int row = 0;
    for (int i = 0; i < 4; i++) {
        int numOfBars = 0;

        if (usages[i] > 0.0) {
            numOfBars = static_cast<int>(usages[i] / 5.0) + 1;
        }

        int colorPair = 3;
        if (usages[i] > 80.0) colorPair = 5;
        else if (usages[i] > 40.0) colorPair = 4;

        mvprintw(row, 0, "%-d[", i + 1);
        for (int j = 0; j < 20; j++) {
            if (j < numOfBars) {
                attron(COLOR_PAIR(colorPair) | A_BOLD);
                mvaddch(row, j + 2, '|');
                attroff(COLOR_PAIR(colorPair) | A_BOLD);
            } else {
                mvaddch(row, j + 2, ' ');
            }

        }
        mvprintw(row, 21, "] %3.1f%%", usages[i]);
        row++;
    }
}