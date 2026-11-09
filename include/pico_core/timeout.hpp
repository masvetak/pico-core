#pragma once

#include <chrono>
#include <functional>

#include "pico/time.h"

class Timeout {
public:
    using Callback = std::function<void()>;

    Timeout() = default;
    ~Timeout();
    Timeout(const Timeout &) = delete;
    Timeout &operator=(const Timeout &) = delete;

    void attach(Callback callback, std::chrono::microseconds delay);
    void detach();

private:
    static int64_t on_alarm(alarm_id_t id, void *context);

    alarm_id_t alarm_id_ = 0;
    Callback callback_;
};
