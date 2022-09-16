#pragma once

#include "embdcorecpp/timer.hpp"
#include "export.hpp"

/**
 * \class Watchdog
 * @brief This class provides the watchdogging.
 */
class EMBDCORECPP_EXPORT Watchdog {
   public:
    /**
     * Create and start watchdogging on a file.
     * @brief Default constructor.
     * @param wdtFile File targeted for watchdodding.
     * @param timeout Watchdog timeout in seconds.
     */
    void fileWatchdog(std::string wdtFile, uint timeout);

   private:
    Timer m_timer;
};
