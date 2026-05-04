#include "fade.hpp"
#include "clock.hpp"

ColorFade::ColorFade() noexcept {
    timer = duration = 0.f;
    enabled = false;
}

void ColorFade::on_update() {
    if (!enabled)
        return;
    timer += gclock->dt;
    if (timer >= duration) {
        cur_color = end_color;
        enabled = false;
        return;
    }
    cur_color = Color(start_color.r + (end_color.r - start_color.r) * timer / duration,
                      start_color.g + (end_color.g - start_color.g) * timer / duration,
                      start_color.b + (end_color.b - start_color.b) * timer / duration,
                      start_color.a + (end_color.a - start_color.a) * timer / duration);
}

void ColorFade::start(Color from, Color to, float dur) {
    start_color = cur_color = from;
    end_color = to;
    enabled = true;
    duration = dur;
    timer = 0.f;
}
