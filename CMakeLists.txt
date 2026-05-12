cmake_minimum_required(VERSION 3.28)
project(CPP_SysMon)

set(CMAKE_CXX_STANDARD 20)

# 1. Kluczowe flagi przed szukaniem pakietu
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)

add_executable(CPP_SysMon
        src/main.cpp
        src/Process.cpp
        src/ProcParser.cpp
        include/ProcParser.h
        include/UserHelper.h
        src/ProcessManager.cpp
        include/ProcessManager.h
        src/DisplayManager.cpp
        include/DisplayManager.h
        include/CpuData.h)

# 2. Poprawiona ścieżka inkludowania
target_include_directories(CPP_SysMon PRIVATE
        include
        ${CURSES_INCLUDE_DIRS} # Zmieniono z _PATH na _DIRS (standard dla find_package)
)

# 3. Dodanie definicji dla szerokich znaków (KLUCZOWE)
# Bez tego ncurses.h może nie udostępnić funkcji dla Unicode
target_compile_definitions(CPP_SysMon PRIVATE
        _GNU_SOURCE
        _XOPEN_SOURCE_EXTENDED
)

# 4. Łączenie z bibliotekami
target_link_libraries(CPP_SysMon PUBLIC ${CURSES_LIBRARIES})