cmake_minimum_required(VERSION 3.17)

add_library(pico_core STATIC
    src/event_queue.cpp
    src/timer.cpp
    src/ticker.cpp
    src/timeout.cpp
)

target_include_directories(pico_core PUBLIC include)
target_compile_features(pico_core PUBLIC cxx_std_17)
target_link_libraries(pico_core PUBLIC pico_stdlib pico_time hardware_sync)
