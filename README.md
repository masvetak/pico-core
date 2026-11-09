# PicoCore

Reusable C++ components for Raspberry Pi Pico, built on the Pico SDK. The
calling application can use FreeRTOS to dispatch events on a task.

## Components

- `Timer`: stopwatch with `start`, `stop`, `reset`, `elapsed_time`, and `read` methods.
- `Ticker`: periodic alarm with `attach` and `detach`.
- `Timeout`: one-shot alarm with `attach` and `detach`.
- `EventQueue`: fixed-size callback queue; `event(callback)` returns a small
  callback that posts work for later `dispatch_pending()`.
- `EventQueueHolder`: singleton owning the main EventQueue.

`Ticker` and `Timeout` callbacks run in an alarm interrupt. Wrap application
work in `EventQueueHolder::getInstance().getMainEventQueue().event(callback)`
when it should run on the task that calls `dispatch_pending()`.

## CMake

Initialize the Pico SDK before adding this directory:

```cmake
pico_sdk_init()
add_subdirectory(submodules/pico-core)
target_link_libraries(your_firmware PRIVATE pico_core)
```
