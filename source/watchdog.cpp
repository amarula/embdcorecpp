#include "embdcorecpp/watchdog.hpp"

#include <fstream>

void Watchdog::fileWatchdog(std::string wdtFile, uint timeout) {
    m_timer.create(0, timeout * 1000, [wdtFile] {
        if (!std::ifstream(wdtFile.c_str()).good()) {
            std::ofstream(wdtFile.c_str()).close();
        }
    });
}
