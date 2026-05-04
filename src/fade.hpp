#pragma once
#include "color.hpp"

class ColorFade {
    Color start_color;
    Color end_color;
    float timer;
    float duration;
    bool enabled;

public:
    Color cur_color;

    ColorFade();
    void on_update();
    void start(Color from, Color to, float dur);
};
