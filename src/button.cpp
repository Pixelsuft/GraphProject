#include "button.hpp"
#include "render.hpp"

Button::Button(std::string id) : Container(id) {
    bg_color = Color(0.f, 0.0f, 0.f);
    border_color = Color(0.f, 1.f, 0.f);
    hover_color = Color(0.f, 1.f, 0.f);
    down_color = Color(0.f, 0.6f, 0.f);
    child = nullptr;
    real_hovered = false;
    refresh_color();
}

Button::~Button() {
    if (child) {
        delete child;
        child = nullptr;
    }
}

void Button::on_update(Container* parent) {
    fade.on_update();
    if (child)
        child->on_update(parent);
}

void Button::on_draw(Container* parent) {
    ren->fill_rect(rect, fade.cur_color);
    if (child)
        child->on_draw(parent);
    ren->draw_rect(rect, border_color);
}

void Button::refresh_color() {
    if (real_hovered)
        return;
    fade.start(bg_color, bg_color, 0.f);
}

void Button::on_mouse_enter(Container* parent, Point pos, bool entered) {
    fade.start(fade.cur_color, entered ? hover_color : bg_color, 0.1f);
}

void Button::on_mouse_down(Container* parent, Point pos, uint8_t index, bool down) {
    real_hovered = has_mouse_collision(parent, pos);
    fade.start(fade.cur_color, down ? down_color : hover_color, 0.1f);
    if (!down && real_hovered && onClick) {
        onClick(this, parent);
    }
    if (!down)
        real_hovered = false;
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

void Button::on_resize(Container* parent) {
    Container::on_resize(parent);
    if (child) {
        child->rect = rect;
        child->on_resize(parent);
    }
}

Button* Button::set_click_handler(std::function<void(Button*, Container*)> handler) {
    onClick = std::move(handler);
    return this;
}

Button* Button::set_child(Container* cont) {
    child = cont;
    if (child) {
        child->rect = rect;
        // child->on_resize(nullptr);
    }
    return this;
}

Container* Button::set_rect(Rect new_rect) {
    Container::set_rect(new_rect);
    if (child) {
        child->rect = new_rect;
        // child->on_resize(nullptr);
    }
    return this;
}
