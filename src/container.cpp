#include "container.hpp"

Container::Container(std::string id) : id(id), visible(true), enabled(true) {}

Container::~Container() {}

void Container::on_resize(Container* parent) {
    if (onResize)
        onResize(this, parent);
}

void Container::on_update(Container* parent) {}

void Container::on_draw(Container* parent) {}

void Container::on_mouse_move(Container* parent, Point pos, Point dp, bool holding) {}

void Container::on_mouse_enter(Container* parent, Point pos, bool entered) {}

void Container::on_mouse_down(Container* parent, Point pos, uint8_t index, bool down) {}

bool Container::has_mouse_collision(Container* parent, Point pos) {
    if (pos.x < 0.f || pos.y < 0.f)
        return false;
    return pos.x >= rect.x && pos.y >= rect.y && pos.x < (rect.x + rect.w) &&
           pos.y < (rect.y + rect.h);
}

Container* Container::set_rect(Rect new_rect) {
    rect = new_rect;
    return this;
}

Container* Container::set_resize_handler(std::function<void(Container*, Container*)> handler) {
    onResize = std::move(handler);
    return this;
}
