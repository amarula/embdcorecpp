#pragma once

#include <functional>
#include <thread>

#include "export.hpp"

/**
 * @brief Service class used to detect when system date/time changes
 */
class EMBDCORECPP_EXPORT TimeChangeService {
   public:
    /**
     * Create service.
     * @brief Default constructor.
     * @param timeChangeCallback Callback executed when changes are detected.
     */
    TimeChangeService(const std::function<void()> &timeChangeCallback);
};
