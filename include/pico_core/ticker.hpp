#pragma once

#include <chrono>
#include <functional>

#include "pico/time.h"

class Ticker {
public:
    using Callback = std::function<void()>;

    Ticker() = default;
    ~Ticker();
    Ticker(const Ticker &) = delete;
    Ticker &operator=(const Ticker &) = delete;

    void attach(Callback callback, std::chrono::microseconds period);
    void detach();

private:
    static bool on_tick(repeating_timer_t *timer);

    repeating_timer_t timer_ = {};
    Callback callback_;
    bool attached_ = false;
};
