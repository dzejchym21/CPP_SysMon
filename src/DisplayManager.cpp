#include "DisplayManager.h"

#include <cstring>
#include <curses.h>
#include <vector>


DisplayManager::DisplayManager() {
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
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

    auto drawCol = [&](const char* label, int width, SortCategory colCat, bool leftAlign) {
        int pair = (currentSort == colCat && colCat != SortCategory::OTHER) ? 2 : 1;

        attron(COLOR_PAIR(pair) | A_BOLD);

        for (int i = 0; i < width; i++) {
            mvaddch(0, currentX + i, ' ');
        }

        const char* end = " ";
        if (pair == 2) {
            end = ascending ? "\u25B2" : "\u25BC";
        }

        if (leftAlign) {
            mvprintw(0, currentX , "%.*s", width, label);
        } else {
            int len = strlen(label);
            int startPos = (len > width) ? currentX : currentX + (width - len);
            mvprintw(0, startPos, "%.*s", width, label);
        }

        currentX += width;
        mvaddstr(0, currentX, end);
        currentX += 1;
        attroff(COLOR_PAIR(pair) | A_BOLD);
    };

    drawCol("PID", 8, SortCategory::PID, false);
    drawCol("PPID", 7, SortCategory::OTHER, false);
    drawCol("USER", 13, SortCategory::USER, true);
    drawCol("NICE", 8, SortCategory::OTHER, false);
    drawCol("ST", 2, SortCategory::STATE, false);
    drawCol("RES (kB)", 9, SortCategory::MEM, false);
    drawCol("%CPU", 5, SortCategory::CPU, false);
    drawCol("COMMAND", 20, SortCategory::NAME, true);
    //mvprintw(0, 0, "%8s %8s %-15s %-10s %8s %-12s %-5s",
    //        "PID", "PPID", "COMMAND", "USER", "NICE", "RES (kB)", "%CPU");

    attroff(COLOR_PAIR(1) | A_BOLD);
}


void DisplayManager::render(const std::vector<const Process*>& processes, SortCategory currentSort, bool ascending) {
    erase();
    //clear();
    drawHeader(currentSort, ascending);

    int row = 1;
    int maxRows = LINES - 1;
    for (const auto* proc : processes) {
        if (row > maxRows) break;

        mvprintw(row, 0, "%8d %8d %-13.13s %8d %2c %9ld %5.2f %-15.15s",
            proc->getPid(),
            proc->getPpid(),
            proc->getUser().c_str(),
            proc->getNice(),
            proc->getState(),
            proc->getMemoryUsage(),
            proc->getCpuUsage(),
            proc->getName().c_str());

        row++;
    }
    refresh();
}