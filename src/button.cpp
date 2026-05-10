#include "button.hpp"
#include "render.hpp"
#include <SDL3/SDL.h>

Button::Button(std::string id) : Container(id) {
    bg_color = Color();
    border_color = Color(0.f, 1.f, 0.f);
    hover_color = Color(0.f, 1.f, 0.f);
    down_color = Color(0.f, 0.6f, 0.f);
    real_hovered = false;
    refresh_color();
}

Button::~Button() {}

void Button::on_update(Container* parent) { fade.on_update(); }

void Button::on_draw(Container* parent) {
    ren->fill_rect(rect, fade.cur_color);
    ren->draw_rect(rect, border_color);
}

void Button::refresh_color() { fade.start(bg_color, bg_color, 0.f); }

void Button::on_mouse_enter(Container* parent, Point pos, bool entered) {
    fade.start(fade.cur_color, entered ? hover_color : bg_color, 0.1f);
}

void Button::on_mouse_down(Container* parent, Point pos, uint8_t index, bool down) {
    real_hovered = has_mouse_collision(parent, pos);
    fade.start(fade.cur_color, down ? down_color : hover_color, 0.1f);
    if (!down && real_hovered) {
        // TODO: click
    }
}

void Button::on_mouse_move(Container* parent, Point pos, Point dp, bool holding) {
    if (!holding)
        return;
    if (has_mouse_collision(parent, pos)) {
        if (!real_hovered) {
            real_hovered = true;
            fade.start(fade.cur_color, down_color, 0.1f);
        }
    } else if (real_hovered) {
        real_hovered = false;
        fade.start(fade.cur_color, bg_color, 0.1f);
    }
}
