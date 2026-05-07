# CPP_SysMon - High-Performance Linux System Monitor

A lightweight, real-time system monitor for Linux written in **C++20** using **ncurses**.
Designed for efficiency and low CPU overhead, featuring a dynamic multi-core dashboard and
stable process management.

## 🚀 Features
* **Multi-core CPU Dashboard:** Real-time usage tracking for all logical cores (tested up to 16 cores).
* **Smart Process Management:** Efficient process parsing via `/proc` filesystem.
* **Optimized Performance:** Uses pointer-to-member projections and `std::ranges` for ultra-fast sorting.
* **Stable Memory Management:** Built with `std::map` to ensure pointer stability and prevent segmentation faults during high-frequency refreshes.
* **Interactive UI:** * Sort by **CPU usage**, **Memory**, **PID**, **User**, or **Command**.
    * Toggle between ascending and descending order.
    * Adaptive refresh rate.

## 🛠 Tech Stack
* **Language:** C++20
* **Library:** `ncurses` (Terminal User Interface)
* **OS:** Linux / WSL2
* **Build System:** CMake

## 📋 Prerequisites
Ensure you have `ncurses` installed on your system:
```bash
sudo apt-get install libncurses5-dev libncursesw5-dev