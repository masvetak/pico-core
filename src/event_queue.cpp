#include "pico_core/event_queue.hpp"

#include <utility>

#include "hardware/sync.h"
#include "pico/stdlib.h"

EventQueue::Callback EventQueue::event(Callback callback) {
    hard_assert(callback_count_ < kMaxCallbacks);
    const uint8_t id = callback_count_++;
    callbacks_[id] = std::move(callback);
    return [this, id] { post(id); };
}

void EventQueue::post(uint8_t callback_id) {
    const uint32_t irq_state = save_and_disable_interrupts();
    if (count_ == kCapacity) {
        // Count repeated events so RM5 pulses survive a temporarily full FIFO.
        hard_assert(overflow_[callback_id] != UINT32_MAX);
        ++overflow_[callback_id];
    } else {
        events_[tail_] = callback_id;
        tail_ = (tail_ + 1) % kCapacity;
        ++count_;
    }
    restore_interrupts(irq_state);
}

void EventQueue::dispatch_pending() {
    while (true) {
        const uint32_t irq_state = save_and_disable_interrupts();
        uint8_t id = 0;
        bool found = false;
        if (count_ > 0) {
            id = events_[head_];
            head_ = (head_ + 1) % kCapacity;
            --count_;
            found = true;
        } else {
            for (uint8_t i = 0; i < callback_count_; ++i) {
                if (overflow_[i] > 0) {
                    --overflow_[i];
                    id = i;
                    found = true;
                    break;
                }
            }
        }
        restore_interrupts(irq_state);
        if (!found) return;
        callbacks_[id]();
    }
}
