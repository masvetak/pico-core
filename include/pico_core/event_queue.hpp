#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>

// A callback wrapper posts an event from an IRQ; dispatch_pending runs the
// registered callback from the application task calling dispatch_pending().
class EventQueue {
public:
    using Callback = std::function<void()>;

    Callback event(Callback callback);
    void dispatch_pending();

private:
    static constexpr std::size_t kMaxCallbacks = 16;
    static constexpr std::size_t kCapacity = 128;

    void post(uint8_t callback_id);

    Callback callbacks_[kMaxCallbacks];
    uint8_t events_[kCapacity] = {};
    uint32_t overflow_[kMaxCallbacks] = {};
    uint8_t callback_count_ = 0;
    volatile std::size_t head_ = 0;
    volatile std::size_t tail_ = 0;
    volatile std::size_t count_ = 0;
};
