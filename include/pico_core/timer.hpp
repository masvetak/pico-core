#pragma once

#include <chrono>
#include <cstdint>

class Timer {
public:
    void start();
    void stop();
    void reset();
    std::chrono::microseconds elapsed_time() const;
    float read() const;
    int read_ms() const;
    int read_us() const;

private:
    uint64_t elapsed_us_ = 0;
    uint64_t started_us_ = 0;
    bool running_ = false;
};
