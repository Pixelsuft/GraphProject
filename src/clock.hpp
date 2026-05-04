#pragma once
#include <cstdint>

class Clock {
    double freq;
    uint64_t last_time;

public:
    double double_dt;
    float dt;
    int fps;

    Clock();
    void on_update();
};

extern Clock* clock;
