#include "pico_core/timeout.hpp"

#include <utility>

#include "hardware/sync.h"
#include "pico/stdlib.h"

Timeout::~Timeout() { detach(); }

void Timeout::attach(Callback callback, std::chrono::microseconds delay) {
    hard_assert(callback && delay.count() > 0);
    detach();
    const uint32_t irq_state = save_and_disable_interrupts();
    callback_ = std::move(callback);
    const alarm_id_t id = add_alarm_in_us(delay.count(), on_alarm, this, false);
    hard_assert(id > 0);
    alarm_id_ = id;
    restore_interrupts(irq_state);
}

void Timeout::detach() {
    const uint32_t irq_state = save_and_disable_interrupts();
    if (alarm_id_ > 0) {
        cancel_alarm(alarm_id_);
        alarm_id_ = 0;
    }
    callback_ = nullptr;
    restore_interrupts(irq_state);
}

int64_t Timeout::on_alarm(alarm_id_t id, void *context) {
    auto *self = static_cast<Timeout *>(context);
    if (self->alarm_id_ == id) {
        self->alarm_id_ = 0;
        if (self->callback_) self->callback_();
    }
    return 0;
}
