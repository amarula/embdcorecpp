#pragma once

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <set>
#include <thread>
#include <unordered_map>

#include "export.hpp"

/**
 * @brief Timer class used for delayed or repetitive tasks.
 */
class EMBDCORECPP_EXPORT Timer {
   public:
    using timer_id = uint64_t;
    using handler_type = std::function<void()>;

    /**
     * Create a new Timer.
     * @brief Default constructor.
     */
    Timer();
    ~Timer();

    /**
     * @brief Generate a new Timer.
     * @param when After how much time(ms) the handler needs to be executed.
     * @param period How much time(ms) the handler needs to be executed
     * repetitively.
     * @param handler The block of that will be executed.
     * @return The timer ID of newly created timer.
     * @see create(uint64_t when, uint64_t period, handler_type&& handler)
     */
    timer_id create(uint64_t when, uint64_t period,
                    const handler_type& handler);

    /**
     * @brief Generate a new Timer.
     * @param when After how much time(ms) the handler needs to be executed.
     * @param period How much time(ms) the handler needs to be executed
     * repetitively.
     * @param handler The block of that will be executed.
     * @return The timer ID of newly created timer.
     * @see create(uint64_t when, uint64_t period, const handler_type& handler)
     */
    timer_id create(uint64_t when, uint64_t period, handler_type&& handler);

    /**
     * @brief Destroy timer generated with create method.
     * @param id Timer id.
     * @return true if timer exists, false otherwise.
     */
    bool destroy(timer_id id);

    /**
     * @brief Check if timer with id exists.
     * @param id Timer id.
     * @return true if timer exists, false otherwise.
     */
    bool exists(timer_id id);

   private:
    std::mutex sync;
    using ScopedLock = std::unique_lock<std::mutex>;

    std::condition_variable wakeUp;

    using Clock = std::chrono::steady_clock;
    using Timestamp = std::chrono::time_point<Clock>;
    using Duration = std::chrono::milliseconds;

    struct Instance {
        Instance(timer_id id = 0) : id(id), running(false) {}

        template <typename Tfunction>
        Instance(timer_id id, Timestamp next, Duration period,
                 Tfunction&& handler) noexcept
            : id(id),
              next(next),
              period(period),
              handler(std::forward<Tfunction>(handler)),
              running(false) {}

        Instance(Instance const& r) = delete;

        Instance(Instance&& r) noexcept
            : id(r.id),
              next(r.next),
              period(r.period),
              handler(std::move(r.handler)),
              running(r.running) {}

        Instance& operator=(Instance const& r) = delete;

        Instance& operator=(Instance&& r) noexcept {
            if (this != &r) {
                id = r.id;
                next = r.next;
                period = r.period;
                handler = std::move(r.handler);
                running = r.running;
            }
            return *this;
        }

        timer_id id;
        Timestamp next;
        Duration period;
        handler_type handler;
        bool running{};
    };

    using InstanceMap = std::unordered_map<timer_id, Instance>;
    timer_id nextId;
    InstanceMap active;

    // Comparison functor to sort the timer "queue" by Instance::next
    struct NextActiveComparator {
        bool operator()(const Instance& a, const Instance& b) const {
            return a.next < b.next;
        }
    };
    NextActiveComparator comparator;

    // Queue is a set of references to Instance objects, sorted by next
    using QueueValue = std::reference_wrapper<Instance>;
    using Queue = std::multiset<QueueValue, NextActiveComparator>;
    Queue queue;

    // Thread and exit flag
    std::thread worker;
    bool done;
    void threadStart();

    timer_id createImpl(Instance&& item);
};
