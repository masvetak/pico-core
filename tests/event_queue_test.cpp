#include "pico_core/event_queue.hpp"

#include <cassert>
#include <vector>

int main() {
    EventQueue queue;
    std::vector<int> values;
    auto fall = queue.event([&] { values.push_back(0); });
    auto rise = queue.event([&] { values.push_back(1); });

    fall();
    rise();
    queue.dispatch_pending();
    assert((values == std::vector<int>{0, 1}));

    values.clear();
    for (int i = 0; i < 130; ++i) (i % 2 ? rise : fall)();
    queue.dispatch_pending();
    assert(values.size() == 130);
    for (int i = 0; i < 128; ++i) assert(values[i] == i % 2);

    values.clear();
    for (int i = 0; i < 260; ++i) fall();
    queue.dispatch_pending();
    assert(values.size() == 260);
}
