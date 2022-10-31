#include "embdcorecpp/timechangeservice.hpp"

#include <sys/timerfd.h>
#include <unistd.h>

#include <climits>
#include <iostream>

TimeChangeService::TimeChangeService(
    const std::function<void()> &timeChangeCallback) {
    std::thread([&]() {
        while (true) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(10s);

            auto tfd = timerfd_create(CLOCK_REALTIME, 0);

            if (tfd > 0) {
                char dummybuf[8];
                struct itimerspec spec = {{0, 0}, {INT_MAX, 0}};
                timerfd_settime(tfd,
                                TFD_TIMER_ABSTIME | TFD_TIMER_CANCEL_ON_SET,
                                &spec, NULL);

                if (read(tfd, &dummybuf, 8) == -1) {
                    if (errno == ECANCELED) {
                        timeChangeCallback();
                    }
                }
                close(tfd);
            }
        }
    }).detach();
}
