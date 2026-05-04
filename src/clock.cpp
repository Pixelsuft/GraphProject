#include "clock.hpp"
#include <SDL3/SDL.h>

Clock::Clock() {
    freq = static_cast<double>(SDL_GetPerformanceFrequency());
    double_dt = 0.0;
    dt = 0.f;
    fps = 0;
    last_time = SDL_GetPerformanceCounter();
}

void Clock::on_update() {
    uint64_t now = SDL_GetPerformanceCounter();
    double_dt = static_cast<double>(now - last_time) / freq;
    last_time = now;
    dt = static_cast<float>(double_dt);
    fps = static_cast<int>(1.0 / double_dt);
    if (static_cast<double>(fps) > freq)
        fps = static_cast<int>(freq);
}
