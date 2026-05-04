#pragma once
#include "color.hpp"

class ColorFade {
public:
    Color cur_color;
    Color start_color;
    Color end_color;
    float timer;
    float duration;
    bool enabled;

    ColorFade() noexcept;
    void on_update();
    void start(Color from, Color to, float dur);
};
