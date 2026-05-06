#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H
#include <ncurses.h>
#include <vector>

#include "ProcessManager.h"

class DisplayManager {
public:
    DisplayManager();
    ~DisplayManager();
    void drawHeader(SortCategory currentSort, bool ascending);
    void render(const std::vector<const Process*>& processes, SortCategory currentSort, bool ascending);
};



#endif //DISPLAYMANAGER_H
