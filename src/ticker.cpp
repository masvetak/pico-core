#include "pico_core/ticker.hpp"

#include <utility>

#include "hardware/sync.h"
#include "pico/stdlib.h"

Ticker::~Ticker() { detach(); }

void Ticker::attach(Callback callback, std::chrono::microseconds period) {
    hard_assert(callback && period.count() > 0);
    detach();
    const uint32_t irq_state = save_and_disable_interrupts();
    callback_ = std::move(callback);
    attached_ = add_repeating_timer_us(-period.count(), on_tick, this, &timer_);
    restore_interrupts(irq_state);
    hard_assert(attached_);
}

void Ticker::detach() {
    const uint32_t irq_state = save_and_disable_interrupts();
    if (attached_) {
        cancel_repeating_timer(&timer_);
        attached_ = false;
    }
    callback_ = nullptr;
    restore_interrupts(irq_state);
}

bool Ticker::on_tick(repeating_timer_t *timer) {
    auto *self = static_cast<Ticker *>(timer->user_data);
    if (self->callback_) self->callback_();
    return true;
}
