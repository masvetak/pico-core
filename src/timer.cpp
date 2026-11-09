#include "pico_core/timer.hpp"

#include "pico/time.h"

void Timer::start() {
    if (running_) return;
    started_us_ = time_us_64();
    running_ = true;
}

void Timer::stop() {
    if (!running_) return;
    elapsed_us_ += time_us_64() - started_us_;
    running_ = false;
}

void Timer::reset() {
    elapsed_us_ = 0;
    if (running_) started_us_ = time_us_64();
}

std::chrono::microseconds Timer::elapsed_time() const {
    const uint64_t elapsed = elapsed_us_ + (running_ ? time_us_64() - started_us_ : 0);
    return std::chrono::microseconds(elapsed);
}

float Timer::read() const {
    return static_cast<float>(elapsed_time().count()) / 1000000.0f;
}

int Timer::read_ms() const {
    return static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time()).count());
}

int Timer::read_us() const { return static_cast<int>(elapsed_time().count()); }
