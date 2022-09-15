#include "embdcorecpp/timer.hpp"

void Timer::threadStart() {
    ScopedLock lock(sync);

    while (!done) {
        if (queue.empty()) {
            wakeUp.wait(lock);
        } else {
            auto firstInstance = queue.begin();
            Instance& instance = *firstInstance;
            auto now = Clock::now();
            if (now >= instance.next) {
                queue.erase(firstInstance);

                instance.running = true;

                lock.unlock();
                instance.handler();
                lock.lock();

                if (!instance.running) {
                    active.erase(instance.id);
                } else {
                    instance.running = false;

                    if (instance.period.count() > 0) {
                        instance.next = instance.next + instance.period;
                        queue.insert(instance);
                    } else {
                        active.erase(instance.id);
                    }
                }
            } else {
                wakeUp.wait_until(lock, instance.next);
            }
        }
    }
}

Timer::Timer() : nextId(1), queue(comparator), done(false) {
    ScopedLock lock(sync);
    worker = std::thread([this] { threadStart(); });
}

Timer::~Timer() {
    ScopedLock lock(sync);
    done = true;
    wakeUp.notify_all();
    lock.unlock();
    worker.join();
}

Timer::timer_id Timer::create(uint64_t msFromNow, uint64_t msPeriod,
                              const std::function<void()>& handler) {
    return createImpl(Instance(0, Clock::now() + Duration(msFromNow),
                               Duration(msPeriod), handler));
}

Timer::timer_id Timer::create(uint64_t msFromNow, uint64_t msPeriod,
                              std::function<void()>&& handler) {
    return createImpl(Instance(0, Clock::now() + Duration(msFromNow),
                               Duration(msPeriod), std::move(handler)));
}

Timer::timer_id Timer::createImpl(Instance&& item) {
    ScopedLock lock(sync);
    item.id = nextId++;
    auto iter = active.emplace(item.id, std::move(item));
    queue.insert(iter.first->second);
    wakeUp.notify_all();
    return item.id;
}

bool Timer::destroy(timer_id id) {
    ScopedLock lock(sync);
    auto i = active.find(id);
    if (i == active.end()) {
        return false;
    }
    if (i->second.running) {
        // A callback is in progress for this Instance,
        // so flag it for deletion in the worker
        i->second.running = false;
    } else {
        queue.erase(std::ref(i->second));
        active.erase(i);
    }

    wakeUp.notify_all();
    return true;
}

bool Timer::exists(timer_id id) {
    ScopedLock lock(sync);
    return active.find(id) != active.end();
}
