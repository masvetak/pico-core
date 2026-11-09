#pragma once

#include "pico_core/event_queue.hpp"

class EventQueueHolder {
public:
    static EventQueueHolder &getInstance();
    EventQueue &getMainEventQueue();

    EventQueueHolder(const EventQueueHolder &) = delete;
    EventQueueHolder &operator=(const EventQueueHolder &) = delete;

private:
    EventQueueHolder() = default;

    EventQueue main_event_queue_;
};
