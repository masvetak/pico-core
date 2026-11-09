#include "pico_core/event_queue_holder.hpp"

EventQueueHolder &EventQueueHolder::getInstance() {
    static EventQueueHolder holder;
    return holder;
}

EventQueue &EventQueueHolder::getMainEventQueue() { return main_event_queue_; }
