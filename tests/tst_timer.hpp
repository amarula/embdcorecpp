#pragma once

#include <gtest/gtest.h>

#include <embdcorecpp/timer.hpp>

using namespace testing;

TEST(Timer, Create) {
    Timer::timer_id timerId = 0;
    Timer timer;

    timerId = timer.create(0, 0, [] {});
    EXPECT_EQ(timerId, 1);
}

TEST(Timer, Exists) {
    Timer::timer_id timerId = 0;
    Timer timer;

    timerId = timer.create(10 * 1000, 0, [] {});
    EXPECT_EQ(timerId, 1);
    EXPECT_EQ(timer.exists(timerId), true);
}

TEST(Timer, Execution) {
    using namespace std::chrono_literals;

    bool dummy = false;
    Timer::timer_id timerId = 0;
    Timer timer;

    timerId = timer.create(1 * 1000, 0, [&] { dummy = true; });
    EXPECT_EQ(dummy, false);
    EXPECT_EQ(timerId, 1);
    std::this_thread::sleep_for(2s);
    EXPECT_EQ(dummy, true);
}

TEST(Timer, Destroy) {
    using namespace std::chrono_literals;

    bool dummy = false;
    Timer::timer_id timerId = 0;
    Timer timer;

    timerId = timer.create(2 * 1000, 0, [&] { dummy = true; });
    EXPECT_EQ(dummy, false);
    EXPECT_EQ(timerId, 1);
    EXPECT_EQ(timer.destroy(timerId), true);
    std::this_thread::sleep_for(3s);
    EXPECT_EQ(dummy, false);
}
